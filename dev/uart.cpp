#include "uart.h"
#include <iostream>
#include "mmap/uart_reg.h"
#include "util/util.h"

int8_t stdin_buff[STDIN_BUFF_SIZE];
int32_t stdin_wptr = 0;
int32_t stdin_rptr = 0;
int8_t stdin_mon_end = 0;

void getch()
{
    system("stty raw -echo");
    while (!stdin_mon_end) {
        if ((stdin_rptr - stdin_wptr + STDIN_BUFF_SIZE) % STDIN_BUFF_SIZE !=
            1)  // non-full
        {
            stdin_buff[stdin_wptr++] = getchar();
            stdin_wptr %= STDIN_BUFF_SIZE;
        }
    }
    system("stty cooked");
}

void Uart::_init() {}

Uart::Uart()
    : txctrl(0),
      rxctrl(0),
      ie(0),
      ip(0),
      div(0),
      txfifo{0},
      tx_rptr(0),
      tx_wptr(0),
      rxfifo{0},
      rx_rptr(0),
      rx_wptr(0),
      t_getch(getch),
      Device(),
      Slave(0x1000),
      IRQSource(-1, NULL)
{
}

Uart::Uart(uint32_t irq_id, PLIC *plic)
    : txctrl(0),
      rxctrl(0),
      ie(0),
      ip(0),
      div(0),
      txfifo{0},
      tx_rptr(0),
      tx_wptr(0),
      rxfifo{0},
      rx_rptr(0),
      rx_wptr(0),
      t_getch(getch),
      Device(),
      Slave(0x1000),
      IRQSource(irq_id, plic)
{
}

Uart::~Uart()
{
    printf("\r\nPress any key to exit ... \r\n");
    stdin_mon_end = 1;
    t_getch.join();
}

void Uart::run()
{
    if (txctrl & UART_TXEN) {
        if (tx_rptr - tx_wptr) {
            if (txfifo[tx_rptr] == '\n')
                putchar('\r');
            putchar((uint8_t) txfifo[tx_rptr++]);
            tx_rptr %= UART_TXFIFO_DEPTH;
        }
    }
    if (rxctrl & UART_RXEN) {
        while (((rx_rptr - rx_wptr + UART_RXFIFO_DEPTH) % UART_RXFIFO_DEPTH !=
                1) &&
               (stdin_rptr - stdin_wptr)) {
            // printf("[DEBUG] rx_rptr = %d, rx_wptr = %d, stdin_rptr = %d,
            // stdin_wptr = %d\r\n", rx_rptr, rx_wptr, stdin_rptr, stdin_wptr);
            // printf("[DEBUG] %d\r\n", (rx_rptr - rx_wptr + UART_RXFIFO_DEPTH)
            // % UART_RXFIFO_DEPTH); for (int i = 0; i < UART_RXFIFO_DEPTH; ++i)
            //     printf("%d: 0x%x\r\n", i, rxfifo[i]);
            rxfifo[rx_wptr++] = stdin_buff[stdin_rptr++];
            rx_wptr %= UART_RXFIFO_DEPTH;
            stdin_rptr %= STDIN_BUFF_SIZE;
        }
    }
    if (ie) {
        if (rx_wptr - rx_rptr)
            DEV_RISING_IRQ()
        else
            DEV_FALLING_IRQ()
    }
}

bool Uart::write(const Addr &addr,
                 const DataType &data_type,
                 const uint64_t &wdata)
{
    uint64_t mask, _wdata;

    switch (data_type) {
    case DATA_TYPE_DWORD:
        mask = -1UL;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        mask = -1U;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        mask = 0xffff;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        mask = 0xff;
        break;
    default:
        abort();
    }

    _wdata = wdata & mask;

    // printf("write [%lx] = %lx\n", addr, _wdata);
    switch (addr) {
    case RG_TXFIFO:
        if (txctrl & UART_TXEN) {
            if ((tx_rptr - tx_wptr + UART_TXFIFO_DEPTH) % UART_TXFIFO_DEPTH !=
                1) {  // check non-full
                txfifo[tx_wptr++] = _wdata;
                tx_wptr %= UART_TXFIFO_DEPTH;
            }
        }
        break;
    case RG_RXFIFO:
        break;
    case RG_TXCTRL:
        txctrl = _wdata;
        break;
    case RG_RXCTRL:
        rxctrl = _wdata;
        break;
    case RG_IE:
        ie = _wdata;
        break;
    case RG_IP:
        break;
    case RG_DIV:
        div = _wdata;
        break;
    }

    return 1;
}

bool Uart::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0L;
    switch (addr) {
    case RG_TXFIFO:
        rdata = ((tx_rptr - tx_wptr) % UART_TXFIFO_DEPTH == 1) ? -1 : 0;
        break;
    case RG_RXFIFO:
        if (rxctrl & UART_RXEN) {
            if (rx_rptr - rx_wptr) {  // check non-empty
                rdata = rxfifo[rx_rptr++];
                rx_rptr %= UART_RXFIFO_DEPTH;
            } else {
                rdata = -1;
            }
        }
        break;
    case RG_TXCTRL:
        rdata = txctrl;
        break;
    case RG_RXCTRL:
        rdata = rxctrl;
        rxctrl = -1;
        break;
    case RG_IE:
        rdata = ie;
        break;
    case RG_IP:
        rdata = ip;
        break;
    case RG_DIV:
        rdata = div;
        break;
    }

    switch (data_type) {
    case DATA_TYPE_DWORD:
        break;
    case DATA_TYPE_WORD:
        rdata = sext(rdata, 32);
        break;
    case DATA_TYPE_WORD_UNSIGNED:
        rdata = zext(rdata, 32);
        break;
    case DATA_TYPE_HWORD:
        rdata = sext(rdata, 16);
        break;
    case DATA_TYPE_HWORD_UNSIGNED:
        rdata = zext(rdata, 16);
        break;
    case DATA_TYPE_BYTE:
        rdata = sext(rdata, 8);
        break;
    case DATA_TYPE_BYTE_UNSIGNED:
        rdata = zext(rdata, 8);
        break;
    default:
        abort();
    }

    // printf("read [%lx] = %lx\n", addr, rdata);

    return 1;
}
