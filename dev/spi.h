#ifndef __SPI__
#define __SPI__
#include "bus/slave.h"
#include "dev/device.h"
#include "dev/irqsrc.h"
#include "dev/plic.h"

#define RX_FIFO_EMPTY() (rx_fifo_rptr == rx_fifo_wptr)
#define RX_FIFO_CLEAR()              \
    do {                             \
        rx_fifo_rptr = rx_fifo_wptr; \
    } while (0)

#define RX_FIFO_POP(__VAR__)                                        \
    do {                                                            \
        __VAR__ = (RX_FIFO_EMPTY() ? -1 : rx_fifo[rx_fifo_rptr++]); \
        rx_fifo_rptr %= RX_FIFO_SIZE;                               \
    } while (0)

#define RX_FIFO_PUSH(__VAL__)              \
    do {                                   \
        rx_fifo[rx_fifo_wptr++] = __VAL__; \
        rx_fifo_wptr %= RX_FIFO_SIZE;      \
    } while (0)

#define RX_FIFO_SIZE 1024

class SPI : public Device, public Slave, public IRQSource
{
    virtual void _init();
    void _update();
    void _read_block(uint32_t sector);
    void _cmd_handler(uint64_t cmd);
    uint8_t cr1_cpha;
    uint8_t cr1_cpol;
    uint8_t cr1_mstr;
    uint8_t cr1_br;
    uint8_t cr1_spe;
    uint8_t cr1_lsbfirst;
    uint8_t cr1_dff;
    uint8_t cr2_rxdmaen;
    uint8_t cr2_txdmaen;
    uint8_t cr2_ssoe;
    uint8_t ie;
    uint8_t ip;
    uint8_t sr_chside;
    uint8_t sr_udr;
    uint8_t sr_crcerr;
    uint8_t sr_modf;
    uint8_t sr_ovr;
    uint8_t sr_bsy;
    uint16_t rx_buff;
    uint64_t tx_buff;
    uint8_t rx_fifo[RX_FIFO_SIZE];
    uint16_t rx_fifo_rptr;
    uint16_t rx_fifo_wptr;
    uint8_t cmd_mask;

    uint8_t app_cmd;
    uint8_t rd_mblk;
    uint32_t sector;

public:
    SPI(uint32_t irq_id = -1, PLIC *plic = NULL);
    ~SPI();
    virtual void single_step();
    virtual void run();
    virtual void stop();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
};


#endif
