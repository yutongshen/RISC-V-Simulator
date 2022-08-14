#include "spi.h"
#include <assert.h>
#include <unistd.h>
#include "mmap/spi_reg.h"
#include "util/util.h"

#define CRC16_POLY 0x1021
#define SD_MODE1

static uint16_t crc_tab[256];
static uint16_t crc = 0;

static void init_crc_tab()
{
    int16_t crc;
    uint8_t c = 0, i;

    do {
        crc = c << 8;
        for (i = 0; i < 8; ++i)
            crc = (crc << 1) ^ ((crc >> 15) & CRC16_POLY);
        crc_tab[c] = crc;
    } while (++c);
}

static inline uint16_t reset_crc() {
    return crc = 0;
}

static inline uint16_t cal_crc(uint8_t c) {
    return crc = (crc << 8 | c) ^ crc_tab[crc >> 8];
}

static inline uint16_t get_crc() {
    return crc;
}

void SPI::_init()
{
    init_crc_tab();
}

void SPI::_update()
{
    if (ie & ip)
        DEV_RISING_IRQ();
    else
        DEV_FALLING_IRQ();
}

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
      ip(1 << 2),  // {sr_txe, sr_rxne, sr_error}
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
      rd_mblk(0),
      wr_mblk(0),
      wr_sblk(0),
      is_wr_blk(0),
      is_wr_crc(0),
      sector(0),
      wr_buff{0},
      wr_idx(0),
#ifdef SD_MODE1
      sd_img(fopen("build/prog/sd_image/sd_image", "rb+")),
#else
      sd_img(NULL),
#endif
      Device(),
      Slave(0x1000),
      IRQSource(irq_id, plic)
{
    _init();
}

SPI::~SPI() {}

void SPI::_cmd_handler(uint64_t data)
{
    uint8_t cmd;
    uint32_t arg;
    uint8_t crc;

    data >>= 16;
    cmd_mask = cmd_mask ? cmd_mask - 1 : cmd_mask;
    if (wr_mblk | wr_sblk) {
        if (!is_wr_blk) {
            switch (data & 0xff) {
                case 0xfc:
                    is_wr_blk = 1;
                    wr_idx = 0;
                    is_wr_crc = 0;
                    break;
                case 0xfd:
                    wr_mblk = 0;
                    for (int i = 0; i < 18; ++i)
                        RX_FIFO_PUSH(0x00);
                    break;
            }
        }
        else if (!is_wr_crc) {
            wr_buff[wr_idx++] = data & 0xff;
            if (wr_idx == 512) {
                is_wr_crc = 1;
            }
        }
        else {
            if (++wr_idx == 514) {
                is_wr_blk = 0;
                wr_sblk = 0;
                _write_block(sector++, wr_buff);
                RX_FIFO_PUSH(0xe5);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
            }
        }
    }
    else if (((data >> 46) & 3) == 1 && (data & 1) == 1 && !cmd_mask) {
        cmd_mask = 6;  // skip 6 iteration
        cmd = (data >> 40) & 0x3f;
        arg = (data >> 8) & 0xffffffff;
        crc = (data >> 0) & 0xff;
        // printf("Recieve %sCMD: %d, ARG: %x, CRC: %x\r\n", app_cmd ? "A" : "",
        //        cmd, arg, crc);
        if (rd_mblk) {
            if (cmd == 12) {
                rd_mblk = 0;
                RX_FIFO_CLEAR();
                RX_FIFO_PUSH(0x3f);
            }
        }
        else if (app_cmd) {
            app_cmd = 0;
            switch (cmd) {
            case 13:
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xff);
                RX_FIFO_PUSH(0xff);
                RX_FIFO_PUSH(0xfe);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x05);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x04);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x90);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x08);
                RX_FIFO_PUSH(0x11);
                RX_FIFO_PUSH(0x39);
                RX_FIFO_PUSH(0x0a);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x08);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x69);
                RX_FIFO_PUSH(0x4e);
            case 41:
                RX_FIFO_PUSH(arg != 0x40000000);
                break;
            case 51:
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xff);
                RX_FIFO_PUSH(0xfe);
                RX_FIFO_PUSH(0x02);
                RX_FIFO_PUSH(0x35);
                RX_FIFO_PUSH(0x84);
                RX_FIFO_PUSH(0x43);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x82);
                RX_FIFO_PUSH(0xb7);
                break;
            default:
                RX_FIFO_PUSH(0x05);
                break;
            }
        } else {
            app_cmd = 0;
            switch (cmd) {
            case 0:
                RX_FIFO_PUSH(1);
                break;
            case 1:
                RX_FIFO_PUSH(1);
                break;
            case 6:
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xff);
                RX_FIFO_PUSH(0xfe);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x64);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x03);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xc7);
                RX_FIFO_PUSH(0xfe);
            case 8:
                RX_FIFO_PUSH(1);
                RX_FIFO_PUSH((arg >> 24) & 0xff);
                RX_FIFO_PUSH((arg >> 16) & 0xff);
                RX_FIFO_PUSH((arg >> 8) & 0xff);
                RX_FIFO_PUSH((arg >> 0) & 0xff);
                break;
            case 9:
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xfe);
                RX_FIFO_PUSH(0x40);
                RX_FIFO_PUSH(0x0e);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x32);
                RX_FIFO_PUSH(0x5b);
                RX_FIFO_PUSH(0x59);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xee);
                RX_FIFO_PUSH(0xbf);
                RX_FIFO_PUSH(0x7f);
                RX_FIFO_PUSH(0x80);
                RX_FIFO_PUSH(0x0a);
                RX_FIFO_PUSH(0x40);
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0x87);
                RX_FIFO_PUSH(0xf3);
                RX_FIFO_PUSH(0x87);
                break;
            case 10:
                RX_FIFO_PUSH(0x00);
                RX_FIFO_PUSH(0xfe);
                RX_FIFO_PUSH(0x74);
                RX_FIFO_PUSH(0x4a);
                RX_FIFO_PUSH(0x60);
                RX_FIFO_PUSH(0x55);
                RX_FIFO_PUSH(0x53);
                RX_FIFO_PUSH(0x44);
                RX_FIFO_PUSH(0x55);
                RX_FIFO_PUSH(0x31);
                RX_FIFO_PUSH(0x20);
                RX_FIFO_PUSH(0x44);
                RX_FIFO_PUSH(0xc6);
                RX_FIFO_PUSH(0x32);
                RX_FIFO_PUSH(0x56);
                RX_FIFO_PUSH(0x01);
                RX_FIFO_PUSH(0x57);
                RX_FIFO_PUSH(0x25);
                RX_FIFO_PUSH(0xc8);
                RX_FIFO_PUSH(0xe8);
                break;
            case 12:
                RX_FIFO_PUSH(0);
                break;
            case 13:
                RX_FIFO_PUSH(0);
                RX_FIFO_PUSH(0);
                break;
            case 16:
                RX_FIFO_PUSH(0);
                break;
            case 17:
                RX_FIFO_PUSH(0x00);
                _read_block(arg);
                break;
            case 18:
                RX_FIFO_PUSH(0x00);
                rd_mblk = 1;
                sector = arg;
                break;
            case 25:
                RX_FIFO_PUSH(0x00);
                wr_mblk = 1;
                sector = arg;
                break;
            case 41:
                RX_FIFO_PUSH(0x01);
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
            case 59:
                RX_FIFO_PUSH(0x00);
                break;
            default:
                RX_FIFO_PUSH(0x05);
                break;
            }
        }
    }

    if (rd_mblk && RX_FIFO_EMPTY()) {
        _read_block(sector++);
    }
}

void SPI::_read_block(uint32_t sector)
{
    uint8_t buff[512];
    uint16_t crc;

#ifdef SD_MODE1
    fseek(sd_img, sector * 512, SEEK_SET);
#else
    char path[128];
    sprintf(path, "build/prog/sd_image/sd_image_%08x.bin", sector);
    sd_img = fopen(path, "rb");
#endif
    // printf("[SPI] read sector: %08x\r\n", sector);
    RX_FIFO_PUSH(0xff);
    RX_FIFO_PUSH(0xff);
    RX_FIFO_PUSH(0xfe);
    if (sd_img != NULL) {
        fread(buff, 512, 1, sd_img);
        reset_crc();
        for (int i = 0; i < 512; ++i) {
            RX_FIFO_PUSH(buff[i]);
            cal_crc(buff[i]);
        }
        cal_crc(0);
        crc = cal_crc(0);
        RX_FIFO_PUSH(crc >> 8);
        RX_FIFO_PUSH(crc & 0xff);
        // printf("[SPI] CRC: %04x\r\n", crc);
#ifndef SD_MODE1
        fclose(sd_img);
#endif
    }
    else {
        for (int i = 0; i < 512; ++i) {
            RX_FIFO_PUSH(0);
        }
        RX_FIFO_PUSH(0);
        RX_FIFO_PUSH(0);
    }
}

void SPI::_write_block(uint32_t sector, uint8_t *buff)
{
#ifdef SD_MODE1
    fseek(sd_img, sector * 512, SEEK_SET);
#else
    char path[128];
    sprintf(path, "build/prog/sd_image/sd_image_%08x.bin", sector);
    sd_img = fopen(path, "wb");
#endif
    printf("[SPI] write sector: %08x\r\n", sector);
    if (sd_img != NULL) {
        fwrite(buff, 512, 1, sd_img);
#ifndef SD_MODE1
        fclose(sd_img);
#endif
    }
}

void SPI::single_step() {}
void SPI::run()  {}
void SPI::stop() {}

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
        cr1_cpha = (_wdata >> 0) & 1;
        cr1_cpol = (_wdata >> 1) & 1;
        if (!cr1_spe) {
            cr1_br = (_wdata >> 3) & 7;
            cr1_lsbfirst = (_wdata >> 7) & 1;
            cr1_dff = (_wdata >> 11) & 1;
        }
        cr1_spe = (_wdata >> 6) & 1;
        break;
    case RG_CR2:
        cr2_rxdmaen = (_wdata >> 0) & 1;
        cr2_txdmaen = (_wdata >> 1) & 1;
        cr2_ssoe = (_wdata >> 2) & 1;
        ie = (_wdata >> 5) & 7;
        break;
    case RG_SR:
        break;
    case RG_DR:
        if (cr1_spe) {
            if (cr2_ssoe) {
                _cmd_handler(tx_buff);
                RX_FIFO_POP(rx_buff);
            } else
                rx_buff = -1;
            ip |= 1 << 1;  // set rxne
            if (cr2_ssoe)
                tx_buff = tx_buff << (8 << cr1_dff) |
                          (_wdata & ((1 << (8 << cr1_dff)) - 1));
        }
        break;
    }

    // printf("Write SPI[%x]: %lx\r\n", addr, _wdata);
    _update();

    return 1;
}

bool SPI::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0L;
    switch (addr) {
    case RG_CR1:
        rdata = cr1_dff << 11 | cr1_lsbfirst << 7 | cr1_spe << 6 | cr1_br << 3 |
                cr1_mstr << 2 | cr1_cpol << 1 | cr1_cpha << 0;
        break;
    case RG_CR2:
        rdata = ie << 5 | cr2_ssoe << 2 | cr2_txdmaen << 1 | cr2_rxdmaen << 0;
        break;
    case RG_SR:
        rdata = sr_bsy << 7 | sr_ovr << 6 | sr_modf << 5 | sr_crcerr << 4 |
                sr_udr << 3 | sr_chside << 2 | ((ip >> 1) & 1) << 1 |
                ((ip >> 2) & 1) << 0;
        break;
    case RG_DR:
        rdata = rx_buff & ((1 << (8 << cr1_dff)) - 1);
        ip &= ~(1 << 1);  // clear rxne;
        // printf("Read SPI[%lx]: %lx, rptr: %d, wptr: %d\r\n", addr, rdata, rx_fifo_rptr, rx_fifo_wptr);
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

    _update();

    // printf("Read SPI[%x]: %lx\r\n", addr, rdata);
    return 1;
}