#ifndef __ETH__
#define __ETH__
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "bus/slave.h"
#include "dev/device.h"
#include "dev/irqsrc.h"
#include "dev/plic.h"

#define ETH_TX_FIFO_SIZE 2048
#define ETH_RX_FIFO_SIZE 2048
#define ETH_RX_LEN_FIFO_SIZE 16

class Eth : public Device, public Slave, public IRQSource
{
    virtual void _init();
    static void recvpacket(uint32_t sock, Eth *eth);

    uint8_t mac_addr[6];

    uint8_t tx_fifo[ETH_TX_FIFO_SIZE];
    uint8_t rx_fifo[ETH_RX_FIFO_SIZE];

    bool    tx_en;
    bool    rx_en;

    uint8_t tx_size;
    uint8_t rx_size;
    
    int16_t  _tx_len;
    int16_t  _rx_len;
    uint16_t tx_len;
    uint16_t rx_len[ETH_RX_LEN_FIFO_SIZE];

    uint16_t tx_wptr, tx_rptr;
    uint16_t rx_wptr, rx_rptr;

    uint16_t rx_len_wptr;
    uint16_t rx_len_rptr;

    uint8_t ie, ip;

    uint8_t wbusy;
    uint8_t rbusy;

    int32_t sock;
    struct ifreq ifr;
    struct sockaddr_ll saddrll;

    bool promisc;

    std::thread *t_recvpacket;

public:
    Eth(uint32_t irq_id = -1, PLIC *plic = NULL);
    Eth(uint32_t irq_id, PLIC *plic, uint8_t *_mac_addr);
    ~Eth();
    virtual void single_step();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
};


#endif
