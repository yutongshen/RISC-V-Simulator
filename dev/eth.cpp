#include "eth.h"
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "mmap/eth_reg.h"
#include "util/util.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define TXEIE_BIT  0
#define RXNEIE_BIT 1

bool sim_end = 0;

void print_packet(char *buff, size_t len) {
    int i, j;
    printf("Package size: %ld\r\n", len);
    for (i = 0; i < len; ++i) {
        printf("%02x ", buff[i]);
        if (i % 16 == 15) {
            for (j = i - 15, printf("  "); j <= i; ++j)
                if (buff[j] >= 0x20 && buff[j] <= 0x7e) putchar(buff[j]);
                else putchar('.');
            puts("");
        }
    }
}

void Eth::_init()
{
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    assert(sock >= 0);
    strncpy(ifr.ifr_name, "ens33", IFNAMSIZ);
    assert(ioctl(sock, SIOCGIFINDEX, &ifr) >= 0);
    saddrll.sll_family = PF_PACKET;
    saddrll.sll_ifindex = ifr.ifr_ifru.ifru_ivalue;
    saddrll.sll_halen = ETH_ALEN;
    t_recvpacket = new std::thread(recvpacket, sock, this);

    ioctl(sock, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags |= IFF_PROMISC;
    ioctl(sock, SIOCSIFFLAGS, &ifr);

    ip |= 1 << TXEIE_BIT;
}

void Eth::recvpacket(uint32_t sock, Eth *eth)
{
    char buff[2048];
    int len, i = 0, j;
    uint8_t bc_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    while (!sim_end) {
        if (!eth->rx_en) {
            usleep(1000);
            continue;
        }
        len = recvfrom(sock, buff, sizeof(buff), 0, NULL, NULL);
        if (len < 42 ||
            len > (eth->rx_rptr + ETH_RX_FIFO_SIZE - eth->rx_wptr - 1) % ETH_RX_FIFO_SIZE ||
            (eth->rx_len_rptr + ETH_RX_LEN_FIFO_SIZE - eth->rx_len_wptr) % ETH_RX_LEN_FIFO_SIZE == 1 ||
            !(eth->promisc || !memcmp(eth->mac_addr, buff, ETH_ALEN) || !memcmp(bc_addr, buff, ETH_ALEN))) {
            continue;
        }
        if (len + eth->rx_wptr > ETH_RX_FIFO_SIZE) {
            memcpy(eth->rx_fifo + eth->rx_wptr, buff, ETH_RX_FIFO_SIZE - eth->rx_wptr);
            memcpy(eth->rx_fifo, buff + ETH_RX_FIFO_SIZE - eth->rx_wptr, len + eth->rx_wptr - ETH_RX_FIFO_SIZE);
        }
        else memcpy(eth->rx_fifo + eth->rx_wptr, buff, len);
        eth->rx_len[eth->rx_len_wptr] = len;
        eth->rx_len_wptr++;
        eth->rx_len_wptr %= ETH_RX_LEN_FIFO_SIZE;
        len = (len + 3) / 4 * 4;
        eth->rx_wptr += len;
        eth->rx_wptr %= ETH_RX_FIFO_SIZE;
    }
}

Eth::Eth(uint32_t irq_id, PLIC *plic)
    : mac_addr{0x00, 0x50, 0, 0, 0, 0},
      tx_fifo{0},
      rx_fifo{0},
      tx_en(0),
      rx_en(0),
      tx_size(2),
      rx_size(2),
      _tx_len(0),
      _rx_len(0),
      tx_len(0),
      rx_len{0},
      tx_wptr(0),
      tx_rptr(0),
      rx_wptr(0),
      rx_rptr(0),
      rx_len_wptr(0),
      rx_len_rptr(0),
      ie(0),
      ip(0),
      wbusy(0),
      rbusy(0),
      promisc(0),
      sock(0),
      ifr{0},
      saddrll{0},
      t_recvpacket(NULL),
      Device(),
      Slave(0x1000),
      IRQSource(irq_id, plic)
{
    srand(time(NULL));
    *(uint32_t *)(mac_addr+2) = rand();
    _init();
}

Eth::Eth(uint32_t irq_id, PLIC *plic, uint8_t *_mac_addr)
    : tx_fifo{0},
      rx_fifo{0},
      tx_en(0),
      rx_en(0),
      tx_size(2),
      rx_size(2),
      _tx_len(0),
      _rx_len(0),
      tx_len(0),
      rx_len{0},
      tx_wptr(0),
      tx_rptr(0),
      rx_wptr(0),
      rx_rptr(0),
      rx_len_wptr(0),
      rx_len_rptr(0),
      ie(0),
      ip(0),
      wbusy(0),
      rbusy(0),
      promisc(0),
      sock(0),
      ifr{0},
      saddrll{0},
      t_recvpacket(NULL),
      Device(),
      Slave(0x1000),
      IRQSource(irq_id, plic)
{
    memcpy(mac_addr, _mac_addr, ETH_ALEN);
    _init();
}

Eth::~Eth()
{
    ioctl(sock, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags &= ~IFF_PROMISC;
    ioctl(sock, SIOCSIFFLAGS, &ifr);
    sim_end = 1;
    close(sock);
    t_recvpacket->join();
    delete t_recvpacket;
}

void Eth::single_step()
{
    wbusy = wbusy ? wbusy - 1 : 0;
    if (rx_len_rptr != rx_len_wptr)
        ip |=   1 << RXNEIE_BIT;
    else
        ip &= ~(1 << RXNEIE_BIT);

    if (ie & ip)
        DEV_RISING_IRQ();
    else
        DEV_FALLING_IRQ();
}

bool Eth::write(const Addr &addr,
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
    case RG_RESET:
        if (_wdata & 1) {
            tx_len = 0;
            tx_wptr = 0;
            tx_rptr = 0;
            rx_wptr = 0;
            rx_rptr = 0;
            rx_len_wptr = 0;
            rx_len_rptr = 0;
            ie = 0;
            wbusy = 0;
            rbusy = 0;
        }
        break;
    case RG_TXLEN:
        if (!wbusy && !tx_len && tx_en && _wdata >= 14 && _wdata <= 1500) {
            // printf("[ETH][DEBUG] write TXLEN\r\n");
            tx_len = _tx_len = _wdata;
        }
        break;
    case RG_TXFIFO:
        if (!wbusy && tx_len && tx_en) {
            switch (tx_size) {
            case 0:
                *( uint8_t *)(tx_fifo + tx_wptr) = _wdata & 0xff;
                break;
            case 1:
                *(uint16_t *)(tx_fifo + tx_wptr) = _wdata & 0xffff;
                break;
            case 2:
                *(uint32_t *)(tx_fifo + tx_wptr) = _wdata & 0xffffffff;
                break;
            }
            _tx_len -= 1 << tx_size;
            tx_wptr += 1 << tx_size;
            if (_tx_len <= 0) {
                // printf("[ETH][DEBUG] sendto\r\n");
                if (tx_rptr + tx_len <= ETH_TX_FIFO_SIZE) {
                    assert(sendto(sock, tx_fifo + tx_rptr, tx_len, 0,
                                  (struct sockaddr*) &saddrll, sizeof(saddrll)) > 0);
                }
                else {
                    char *buff = new char[tx_len];
                    memcpy(buff, tx_fifo + tx_rptr, ETH_TX_FIFO_SIZE - tx_rptr);
                    memcpy(buff + ETH_TX_FIFO_SIZE - tx_rptr, tx_fifo, tx_len + tx_rptr - ETH_TX_FIFO_SIZE);
                    assert(sendto(sock, buff, tx_len, 0,
                                  (struct sockaddr*) &saddrll, sizeof(saddrll)) > 0);
                    delete [] buff;
                }
                tx_len = 0;
                tx_wptr = (tx_wptr + 3) >> 2 << 2;
                tx_wptr %= ETH_TX_FIFO_SIZE;
                tx_rptr = tx_wptr;
                wbusy = 0x7f;
            }
            else tx_wptr %= ETH_TX_FIFO_SIZE;
        }
        break;
    case RG_TXCTRL:
        if (!wbusy && !tx_len) {
            tx_size = (_wdata >> 16) & 0x3;
            promisc = (_wdata >>  1) & 0x1;
            tx_en   = (_wdata >>  0) & 0x1;
        }
        break;
    case RG_TXDIS:
        if (_wdata) {
            tx_len = _tx_len = 0;
            tx_wptr = tx_rptr;
        }
        break;
    case RG_RXLEN:
        break;
    case RG_RXFIFO:
        break;
    case RG_RXCTRL:
        if (!rbusy) {
            // printf("[ETH][DEBUG] set RXCTRL = %lx\r\n", _wdata);
            rx_size = (_wdata >> 16) & 0x3;
            rx_en   = (_wdata >>  0) & 0x1;
        }
        break;
    case RG_RXDIS:
        if (_wdata && rbusy) {
            rbusy = 0;
            rx_rptr += _rx_len > 0 ? rbusy : 0;
            rx_rptr  = (rx_rptr + 3) / 4 * 4;
            rx_rptr %= ETH_RX_FIFO_SIZE;
            rx_len_rptr++;
            rx_len_rptr %= ETH_RX_LEN_FIFO_SIZE;
        }
        break;
    case RG_IE:
        ie = _wdata & (1 << TXEIE_BIT | 1 << RXNEIE_BIT);
        break;
    case RG_IP:
        break;
    case RG_IC:
        break;
    case RG_MAC0:
        break;
    case RG_MAC1:
        break;
    }

    return 1;
}

bool Eth::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0L;
    switch (addr & ~0x3) {
    case RG_RESET:
        rdata = !!wbusy << 31 | !!rbusy << 30;
        break;
    case RG_TXLEN:
        rdata = tx_len;
        break;
    case RG_TXFIFO:
        rdata = tx_len ? 0 : -1;
        break;
    case RG_TXCTRL:
        rdata = tx_size << 16 | promisc << 1 | tx_en << 0;
        break;
    case RG_TXDIS:
        break;
    case RG_RXLEN:
        if (rx_len_rptr != rx_len_wptr) {
            // printf("[ETH][DEBUG] read RXLEN\r\n");
            rdata = rx_len[rx_len_rptr];
            if (!rbusy) {
                _rx_len = rx_len[rx_len_rptr];
                rbusy = 1;
            }
        }
        break;
    case RG_RXFIFO:
        if (rbusy) {
            if (_rx_len > 0) {
                rdata = rx_fifo[rx_rptr];
                if (rx_size > 0) rdata |= rx_fifo[rx_rptr + 1] << 8;
                if (rx_size > 1) rdata |= *(uint16_t *)(rx_fifo + rx_rptr + 2) << 16;
                rx_rptr += 1 << rx_size;
                rx_rptr %= ETH_RX_FIFO_SIZE;
                _rx_len -= 1 << rx_size;
            }
        } 
        break;
    case RG_RXCTRL:
        rdata = rx_size << 16 | rx_en << 0;
        break;
    case RG_RXDIS:
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
    case RG_MAC0:
        rdata = *(uint32_t *)mac_addr;
        break;
    case RG_MAC1:
        rdata = *(uint16_t *)(mac_addr + 4);
        break;
    }

    rdata >>= (addr & 0x3) * 8;
    
    return 1;
}
