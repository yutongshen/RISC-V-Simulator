#include "uart.h"
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "mmap/uart_reg.h"
#include "util/util.h"

extern uint64_t __exit;
int8_t stdin_buff[STDIN_BUFF_SIZE];
int32_t stdin_wptr = 0;
int32_t stdin_rptr = 0;
int8_t stdin_mon_end = 0;

void getch()
{
    uint32_t exit_key = 0;
    system("stty raw -echo");
    while (!stdin_mon_end) {
        if (!FIFO_FULL(stdin, STDIN_BUFF_SIZE)) {
            exit_key = exit_key << 8 | (stdin_buff[stdin_wptr++] = getchar());
            // printf("[DEBUG] exit_key = %08x\n", exit_key);
            stdin_wptr %= STDIN_BUFF_SIZE;
            if (exit_key == (((uint32_t) 'e') << 24 | ((uint32_t) 'x') << 16 |
                             ((uint32_t) 'i') << 8 | ((uint32_t) 't') << 0))
                __exit = 1;
        }
    }
    system("stty -raw echo");
    assert(exit_key != (((uint32_t) 'e') << 24 | ((uint32_t) 'x') << 16 |
                        ((uint32_t) 'i') << 8 | ((uint32_t) 't') << 0));
}

void Uart::_init() {}

Uart::Uart(uint32_t irq_id, PLIC *plic)
    : txctrl(0),
      rxctrl(0),
      nstop(0),
      txcnt(0),
      rxcnt(0),
      ie(0),
      ip(0),
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
    if (isatty(fileno(stdout)))
        setbuf(stdout, NULL);
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
        if (!FIFO_EMPT(tx)) {
            if (txfifo[tx_rptr] == '\n')
                putchar('\r');
            putchar((uint8_t) txfifo[tx_rptr++]);
            tx_rptr %= UART_TXFIFO_DEPTH;
        }
    }
    if (rxctrl & UART_RXEN) {
        while (!FIFO_FULL(rx, UART_RXFIFO_DEPTH) && !FIFO_EMPT(stdin)) {
            // printf("[DEBUG] rx_rptr = %d, rx_wptr = %d, stdin_rptr = %d,
            // stdin_wptr = %d\r\n",
            //         rx_rptr, rx_wptr, stdin_rptr, stdin_wptr);
            // printf("[DEBUG] txwm_ie = %x, rxwm_ie = %x\r\n",
            //         (ie >> 0) & 1, (ie >> 1) & 1);
            // printf("[DEBUG] txwm_ip = %x, rxwm_ip = %x\r\n",
            //         (ip >> 0) & 1, (ip >> 1) & 1);
            // for (int i = 0; i < UART_RXFIFO_DEPTH; ++i)
            //     printf("%d: 0x%x\r\n", i, rxfifo[i]);
            rxfifo[rx_wptr++] = stdin_buff[stdin_rptr++];
            rx_wptr %= UART_RXFIFO_DEPTH;
            stdin_rptr %= STDIN_BUFF_SIZE;
        }
    }
    if ((tx_wptr - tx_rptr + UART_TXFIFO_DEPTH) % UART_TXFIFO_DEPTH <=
        (UART_TXFIFO_DEPTH >> 3) * txcnt) {
        ip |= 1 << 0;
    } else {
        ip &= ~(1 << 0);
    }
    if ((rx_wptr - rx_rptr + UART_RXFIFO_DEPTH) % UART_RXFIFO_DEPTH >
        (UART_RXFIFO_DEPTH >> 3) * rxcnt) {
        ip |= 1 << 1;
    } else {
        ip &= ~(1 << 1);
    }
    if (ie & ip)
        DEV_RISING_IRQ();
    else
        DEV_FALLING_IRQ();
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

    // if (addr != RG_TXFIFO)
    //     printf("[DEBUG] write [%lx] = %lx\r\n", addr, _wdata);

    switch (addr) {
    case RG_TXFIFO:
        if (txctrl & UART_TXEN) {
            if (!FIFO_FULL(tx, UART_TXFIFO_DEPTH)) {
                txfifo[tx_wptr++] = _wdata;
                tx_wptr %= UART_TXFIFO_DEPTH;
            }
        }
        break;
    case RG_RXFIFO:
        break;
    case RG_TXCTRL:
        txctrl = _wdata & (1 << 0);
        nstop = _wdata & (1 << 1);
        txcnt = _wdata & (7 << 16);
        break;
    case RG_RXCTRL:
        rxctrl = _wdata & (1 << 0);
        rxcnt = _wdata & (7 << 16);
        break;
    case RG_IE:
        ie = _wdata & 0x7;
        break;
    case RG_IP:
        break;
    case RG_IC:
        ip &= ~(_wdata & 0x7);
        break;
    case RG_DIV:
        div = _wdata & 0xffff;
        break;
    }

    return 1;
}

bool Uart::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0L;
    switch (addr) {
    case RG_TXFIFO:
        rdata = ((uint64_t) FIFO_FULL(tx, UART_TXFIFO_DEPTH)) << 31;
        break;
    case RG_RXFIFO:
        if (!FIFO_EMPT(rx)) {
            rdata = rxfifo[rx_rptr++];
            rx_rptr %= UART_RXFIFO_DEPTH;
        } else {
            rdata = 1 << 31;
        }
        break;
    case RG_TXCTRL:
        rdata = txcnt << 16 | nstop << 1 | txctrl;
        break;
    case RG_RXCTRL:
        rdata = rxcnt << 16 | rxctrl;
        break;
    case RG_IE:
        rdata = ie;
        break;
    case RG_IP:
        rdata = ip;
        break;
    case RG_IC:
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

    // if (addr != RG_TXFIFO && addr != RG_RXFIFO && addr != RG_IP)
    //     printf("[DEBUG] read [%lx] = %lx\r\n", addr, rdata);

    return 1;
}
