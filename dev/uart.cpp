#include "uart.h"
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "mmap/uart_reg.h"
#include "util/util.h"

extern uint64_t __exit;
extern bool verbose;
int8_t stdin_buff[STDIN_BUFF_SIZE];
int32_t stdin_wptr = 0;
int32_t stdin_rptr = 0;
int8_t stdin_mon_end = 0;

void Uart::getch(Uart *uart)
{
    uint32_t exit_key = 0;
    system("stty raw -echo");
    while (!stdin_mon_end) {
        if (FIFO_FULL(uart->rx, UART_RXFIFO_DEPTH) &&
            (uart->rxctrl & UART_RXEN)) {
            usleep(1);
            continue;
        }

        uint8_t tmp(getchar());
        if (uart->rxctrl & UART_RXEN) {
            uart->rxfifo[uart->rx_wptr++] = tmp;
            uart->rx_wptr %= UART_RXFIFO_DEPTH;
            uart->_update();
        }
        exit_key = exit_key << 8 | tmp;
        if (exit_key == (((uint32_t) 'e') << 24 | ((uint32_t) 'x') << 16 |
                         ((uint32_t) 'i') << 8 | ((uint32_t) 't') << 0))
            __exit = 1;

        if (tmp == '+') {
            verbose = !verbose;
            printf("[DBG] verbose = %x\r\n", verbose);
        }
    }

    printf("[DEBUG] exit = %ld\r\n", __exit);
    system("stty -raw echo");
    assert(exit_key != (((uint32_t) 'e') << 24 | ((uint32_t) 'x') << 16 |
                        ((uint32_t) 'i') << 8 | ((uint32_t) 't') << 0));
}

void Uart::_init() {}

void Uart::_update()
{
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

Uart::Uart(uint32_t irq_id, PLIC *plic)
    : txctrl(0),
      rxctrl(0),
      nstop(0),
      txcnt(0),
      rxcnt(0),
      ie(0),
      ip(1 << 0),
      txfifo{0},
      tx_rptr(0),
      tx_wptr(0),
      rxfifo{0},
      rx_rptr(0),
      rx_wptr(0),
      t_getch(getch, this),
      Device(),
      Slave(0x1000),
      IRQSource(irq_id, plic)
{
    if (isatty(fileno(stdout)))
        setbuf(stdout, NULL);
    _update();
}

Uart::~Uart()
{
    printf("\r\nPress any key to exit ... \r\n");
    stdin_mon_end = 1;
    t_getch.join();
}

void Uart::single_step() {}

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


    switch (addr) {
    case RG_TXFIFO:
        if (!(txctrl & UART_TXEN))
            break;

        _wdata &= 0xff;
        if (_wdata == '\n')
            putchar('\r');
        putchar(_wdata);
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
        _update();
        break;
    case RG_IE:
        ie = _wdata & 0x7;
        _update();
        break;
    case RG_IP:
        break;
    case RG_IC:
        ip &= ~(_wdata & 0x6);
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
    _update();

    return 1;
}
