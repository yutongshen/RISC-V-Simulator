#include "spi.h"
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "mmap/spi_reg.h"
#include "util/util.h"

void SPI::_init() {}

SPI::SPI(uint32_t irq_id, PLIC *plic)
    : cr1_cpha(0),
      cr1_cpol(0),
      cr1_mstr(1),
      cr1_br(0),
      cr1_spe(0),
      cr1_lsbfirst(0),
      cr1_dff(0),
      cr2_rxdmaen(0),
      cr2_txdmaen(0),
      cr2_ssoe(1),
      ie(0),
      ip(1 << 2), // {sr_txe, sr_rxne, sr_error}
      sr_chside(0),
      sr_udr(0),
      sr_crcerr(0),
      sr_modf(0),
      sr_ovr(0),
      sr_bsy(0),
      tx_buff(0),
      rx_buff(-1),
      rx_fifo{0},
      rx_fifo_rptr(0),
      rx_fifo_wptr(0),
      cmd_mask(0),
      app_cmd(0),
      Device(),
      Slave(0x1000),
      IRQSource(irq_id, plic)
{
}

SPI::~SPI()
{
}

void SPI::cmd_handler(uint64_t data) {
    uint8_t  cmd;
    uint32_t arg;
    uint8_t  crc;
    data >>= 16;
    cmd_mask = cmd_mask ? cmd_mask - 1 : cmd_mask;
    if (((data >> 46) & 3) == 1 && (data & 1) == 1 && !cmd_mask) {
        cmd_mask = 6; // skip 6 iteration
        cmd = (data >> 40) & 0x3f;
        arg = (data >>  8) & 0xffffffff;
        crc = (data >>  0) & 0xff;
        // printf("Recieve %sCMD: %d, ARG: %x, CRC: %x\r\n", app_cmd ? "A" : "", cmd, arg, crc);
        if (app_cmd) {
            app_cmd = 0;
            switch (cmd) {
                case 41:
                    RX_FIFO_PUSH(0x00);
                    break;
                default:
                    RX_FIFO_PUSH(0x05);
                    break;
            }
        }
        else {
            app_cmd = 0;
            switch (cmd) {
                case 0:
                    RX_FIFO_PUSH(1);
                    break;
                case 1:
                    RX_FIFO_PUSH(1);
                    break;
                case 5:
                    RX_FIFO_PUSH(5);
                    break;
                case 8:
                    RX_FIFO_PUSH(1);
                    RX_FIFO_PUSH((arg >> 24) & 0xff);
                    RX_FIFO_PUSH((arg >> 16) & 0xff);
                    RX_FIFO_PUSH((arg >>  8) & 0xff);
                    RX_FIFO_PUSH((arg >>  0) & 0xff);
                    break;
                case 16:
                    if (arg == 512)
                        RX_FIFO_PUSH(0);
                    break;
                case 41:
                    RX_FIFO_PUSH(0x01);
                    break;
                case 52:
                    RX_FIFO_PUSH(0x05);
                    break;
                case 55:
                    RX_FIFO_PUSH(0x01);
                    app_cmd = 1;
                    break;
                case 58:
                    RX_FIFO_PUSH(0x00);
                    RX_FIFO_PUSH(0xc0);
                    RX_FIFO_PUSH(0xff);
                    RX_FIFO_PUSH(0x80);
                    RX_FIFO_PUSH(0x00);
                    break;
                default:
                    RX_FIFO_PUSH(0x05);
                    break;
            }
        }
    }
}

void SPI::run()
{
    if (ie & ip)
        DEV_RISING_IRQ();
    else
        DEV_FALLING_IRQ();
}

bool SPI::write(const Addr &addr,
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
    case RG_CR1:
        cr1_cpha     = (_wdata >> 0) & 1;
        cr1_cpol     = (_wdata >> 1) & 1;
        if (!cr1_spe) {
            cr1_br       = (_wdata >> 3) & 7;
            cr1_lsbfirst = (_wdata >> 7) & 1;
            cr1_dff      = (_wdata >> 11) & 1;
        }
        cr1_spe      = (_wdata >> 6) & 1;
        break;
    case RG_CR2:
        cr2_rxdmaen = (_wdata >> 0) & 1;
        cr2_txdmaen = (_wdata >> 1) & 1;
        cr2_ssoe    = (_wdata >> 2) & 1;
        ie          = (_wdata >> 5) & 7;
        break;
    case RG_SR:
        break;
    case RG_DR:
        if (cr1_spe) {
            if (cr2_ssoe) {
                cmd_handler(tx_buff);
                RX_FIFO_POP(rx_buff);
            }
            else rx_buff = -1;
            ip  |= 1 << 1; // set rxne
            if (cr2_ssoe)
                tx_buff = tx_buff << (8 << cr1_dff) | (_wdata & ((1 << (8 << cr1_dff)) - 1));
        }
        break;
    }

    // printf("Write SPI[%x]: %lx\r\n", addr, _wdata);

    return 1;
}

bool SPI::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0L;
    switch (addr) {
    case RG_CR1:
        rdata = cr1_dff << 11 | cr1_lsbfirst << 7 | cr1_spe << 6 | cr1_br << 3 |
                cr1_mstr << 2 | cr1_cpol << 1     | cr1_cpha << 0;
        break;
    case RG_CR2:
        rdata = ie << 5 | cr2_ssoe << 2 | cr2_txdmaen << 1 | cr2_rxdmaen << 0;
        break;
    case RG_SR:
        rdata = sr_bsy << 7 | sr_ovr << 6 | sr_modf << 5 | sr_crcerr << 4 |
                sr_udr << 3 | sr_chside << 2 | ((ip >> 1) & 1) << 1 | ((ip >> 2) & 1) << 0;
        break;
    case RG_DR:
        rdata = rx_buff & ((1 << (8 << cr1_dff)) - 1);
        ip &= ~(1 << 1); // clear rxne;
        // printf("Read SPI[%x]: %lx, rptr: %d, wptr: %d\r\n", addr, rdata, rx_fifo_rptr, rx_fifo_wptr);
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

    // printf("Read SPI[%x]: %lx\r\n", addr, rdata);
    return 1;
}
