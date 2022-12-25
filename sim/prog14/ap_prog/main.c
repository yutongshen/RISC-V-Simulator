#include <stdio.h>
#include <string.h>
// #include "util.h"
#define MTU 1500
#define ETH_HEADER_SIZE   14
#define IP_HEADER_SIZE    20
#define UDP_HEADER_SIZE    8
#define DHCP_HEADER_SIZE 240

#define UDP_PROTO 17

char dst_addrll[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char src_addrll[] = {0x00, 0x50, 0xca, 0xfe, 0xca, 0xfe};

struct ethframe {
    char h_dest[6];
    char h_source[6];
    uint16_t h_proto;
    char data[MTU];
};

struct iphdr {
    uint8_t  ihl:4;
    uint8_t  ver:4;
    uint8_t  tos;
    uint16_t tot_len;
	uint16_t id;
	uint16_t frag_off;
	uint8_t  ttl;
	uint8_t  protocol;
	uint16_t check;
	uint32_t saddr;
	uint32_t daddr;
};

struct udphdr {
	uint16_t source;
	uint16_t dest;
	uint16_t len;
	uint16_t check;
};

struct dhcphdr {
    uint8_t  op;
    uint8_t  htype;
    uint8_t  hlen;
    uint8_t  hops;
    uint32_t xid;
    uint16_t secs;
    uint16_t flags;
    uint32_t ciaddr;
    uint32_t yiaddr;
    uint32_t siaddr;
    uint32_t giaddr;
    uint8_t  chaddr[16];
    uint8_t  sname[64];
    uint8_t  file[128];
    uint32_t mcookie;
};

uint16_t htons(uint16_t x) {
    return (x & 0xff) << 8  | (x & 0xff00) >> 8;
}

uint32_t htonl(uint32_t x) {
    x = (x & 0xffff) << 16 | (x & 0xffff0000) >> 16;
    x = (x & 0xff00ff) << 8 | (x & 0xff00ff00) >> 8;
    return x;
}

int main(int argc, char **argv)
{
    float pi = 3.1415926;
    printf("hello world!!\npi = %f\n", pi);
    printf("MTIP TEST PASS\n", pi);
    return 0;
}

void eth_init()
{
    *ETH_RG_TXCTRL_32P = 2 << 16 | 1;
    *ETH_RG_RXCTRL_32P = 2 << 16 | 1;
}

void sendframe(const char *data, int len, const char *shaddr,
               const char *dhaddr, uint16_t proto)
{
    /* TM_PRINT="Send frame" */
    struct ethframe frame;
    memcpy(frame.h_dest,   dhaddr, 6);
    memcpy(frame.h_source, shaddr, 6);
    frame.h_proto = htons(proto);
    memcpy(frame.data, data, len);

    uint32_t *ptr = (uint32_t *)&frame;
    while (*ETH_RG_RESET_32P >> 31);
    *ETH_RG_TXLEN_32P = len + ETH_HEADER_SIZE;

    /* TM_PRINT="ETH_RG_RESET  = %x", *ETH_RG_RESET_32P */
    /* TM_PRINT="ETH_RG_TXCTRL = %x", *ETH_RG_TXCTRL_32P */
    /* TM_PRINT="ETH_RG_TXLEN  = %x", *ETH_RG_TXLEN_32P */
    for (int i = *ETH_RG_TXLEN_32P; i > 0; i -= 4) *ETH_RG_TXFIFO_32P = *ptr++;
}

void sendip(const char *data, int len, char proto,
            const char *saddr, const char *daddr,
            const char *shaddr, const char *dhaddr) {
    char buff[MTU];
    struct iphdr *packet = buff;

    packet->ver      = 4;
    packet->ihl      = 5;
    packet->tos      = 0;
    packet->tot_len  = htons(IP_HEADER_SIZE + len);
    packet->id       = 0;
    packet->frag_off = 0;
    packet->ttl      = 128;
    packet->protocol = proto;
    packet->check    = 0;
    packet->saddr    = *(uint32_t *) saddr;
    packet->daddr    = *(uint32_t *) daddr;
    memcpy(buff + IP_HEADER_SIZE, data, len);

    // checksum
    uint16_t *ptr = (uint16_t *)buff;
    uint32_t sum = 0;
    for (uint32_t i = 0; i < packet->ihl * 2; ++i) {
        sum += htons(*(ptr++));
    }

    sum = (sum & 0xffff) + (sum >> 16);
    packet->check = htons(~sum);
    
    sendframe((const char *)packet, IP_HEADER_SIZE + len, shaddr, dhaddr, 0x0800);
}

void sendudp(char *data, uint32_t len,
            uint8_t *saddr, uint8_t *daddr,
            uint16_t sport, uint16_t dport,
            uint8_t *shaddr, uint8_t *dhaddr) {
    char buff[MTU - IP_HEADER_SIZE];
    struct udphdr *packet = buff;
    packet->source = htons(sport);
    packet->dest   = htons(dport);
    packet->len    = htons(UDP_HEADER_SIZE + len);

    memcpy(buff + UDP_HEADER_SIZE, data, len);
    
    sendip(buff, UDP_HEADER_SIZE + len, UDP_PROTO, saddr, daddr, shaddr, dhaddr);
}

void dhcpdiscover(char *shaddr) {
    char buff[DHCP_HEADER_SIZE + 32];
    struct dhcphdr *packet = buff;
    char *opt = buff + DHCP_HEADER_SIZE;
    char saddr[] = {0x00, 0x00, 0x00, 0x00};
    char daddr[] = {0xff, 0xff, 0xff, 0xff};
    char dhaddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    memset(buff, 0, DHCP_HEADER_SIZE + 32);
    packet->op    = 1;
    packet->htype = 1;
    packet->hlen  = 6;
    packet->hops  = 0;
    packet->xid   = htonl(0);
    packet->flags = 0;
    memcpy(packet->chaddr, shaddr, 6);
    packet->mcookie = htonl(0x63825363);

    // dhcp discover
    opt[0] = 53;
    opt[1] = 1;
    opt[2] = 1;

    // client identifier
    opt[3] = 61;
    opt[4] = 7;
    opt[5] = 1;
    memcpy(opt + 6, shaddr, 6);

    // req ip
    opt[12] = 50;
    opt[13] = 4;
    opt[14] = 0x00;
    opt[15] = 0x00;
    opt[16] = 0x00;
    opt[17] = 0x00;

    // req list
    opt[18] = 55;
    opt[19] = 4;
    opt[20] = 0x01;
    opt[21] = 0x03;
    opt[22] = 0x06;
    opt[23] = 0x2a;

    // end
    opt[24] = 0xff;
    opt[25] = 0x00;
    opt[26] = 0x00;
    opt[27] = 0x00;
    opt[28] = 0x00;
    opt[29] = 0x00;
    opt[20] = 0x00;
    opt[31] = 0x00;

    sendudp(buff, DHCP_HEADER_SIZE + 32, saddr, daddr, 68, 67, shaddr, dhaddr);
}

char recvframe(char *buff, uint32_t *len, uint32_t retry)
{
    /* TM_PRINT="Receive frame" */
    while (len = *ETH_RG_RXLEN_32P, !len && retry--);
    if (!len) return;
    /* TM_PRINT="get a frame len = %d", len */
    for (int i = 0, *ptr = buff; i < len; i += 4)
        *ptr++ = *ETH_RG_RXFIFO_32P;

    *ETH_RG_RXDIS_32P = 1;

    char res = 0;
    /* TM_PRINT="From: %02x:%02x:%02x:%02x:%02x:%02x", buff[6], buff[7], buff[8], buff[9], buff[10], buff[11] */
    /* TM_PRINT="To:   %02x:%02x:%02x:%02x:%02x:%02x", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5] */
    if (*(uint32_t *)buff == *(uint32_t *)src_addrll &&
        *(uint16_t *)(buff + 4) == *(uint16_t *)(src_addrll + 4)) {
        for (int i = 0, *ptr = buff; i < len; i += 4, ptr++) {
            /* TM_PRINT="%02x %02x %02x %02x", *ptr & 0xff, (*ptr >> 8) & 0xff, (*ptr >> 16) & 0xff, (*ptr >> 24) & 0xff */
        }
        res = 1;
    }
    return res;
}

void eth_test(void)
{ 
    /* TM_PRINT="Ethernet test begin" */
    eth_init();
    
    char data[100] = {"Hello World~ RISC-V~"};
    char saddr[] = {0, 0, 0, 0};
    char daddr[] = {255, 255, 255, 255};
    char buff[1500];
    int len;
    // sendframe(data, strlen(data), src_addrll, dst_addrll, 0x0800);
    // sendip(data, strlen(data), UDP_PROTO, saddr, daddr, src_addrll, dst_addrll);
    // sendudp(data, strlen(data), saddr, daddr, 68, 67, src_addrll, dst_addrll);
    dhcpdiscover(src_addrll);
    while (!recvframe(buff, &len, 20000)) dhcpdiscover(src_addrll);
}
