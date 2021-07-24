#ifndef __UART__
#define __UART__
#include <thread>
#include "bus/slave.h"
#include "dev/device.h"

#define UART_TXEN (1 << 0)
#define UART_RXEN (1 << 0)

#define UART_TXIE (1 << 0)
#define UART_RXIE (1 << 1)

#define UART_RXFIFO_DEPTH 8
#define UART_TXFIFO_DEPTH 8


class Uart : public Device, public Slave
{
    virtual void _init();
    static void getch();
    static int8_t end;

    uint32_t txctrl;
    uint32_t rxctrl;
    uint32_t ie;
    uint32_t ip;
    uint32_t div;

    uint32_t txfifo[UART_TXFIFO_DEPTH];
    int8_t tx_rptr;
    int8_t tx_wptr;

    static uint32_t rxfifo[UART_RXFIFO_DEPTH];
    static int8_t rx_rptr;
    static int8_t rx_wptr;

    std::thread t_getch;

public:
    Uart();
    ~Uart();
    virtual void run();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
};


#endif
