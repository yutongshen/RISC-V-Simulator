#include <stdio.h>
#include <string.h>
#include "util.h"
#include "riscv_def.h"
// #include "util.h"
#define MTU 1500
#define ETH_HEADER_SIZE   14
#define IP_HEADER_SIZE    20
#define UDP_HEADER_SIZE    8
#define DHCP_HEADER_SIZE 240

#define UDP_PROTO 17
#define PERI_PA2KVA(ADDR, TYPE) ((volatile TYPE *)(ADDR - 0x80000000L))

char dst_addrll[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char src_addrll[] = {0x00, 0x50, 0xca, 0xfe, 0xca, 0xfe};

void *frame_handler(trapframe_t *);
extern void *copy_from_user(void *_kern, void *_user, uint32_t _len);
extern void *copy_to_user(void *_kern, void *_user, uint32_t _len);

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

void sendframe(const char *data, int len, const char *shaddr,
               const char *dhaddr, uint16_t proto)
{
    printf("[U-mode] Send frame\n");
    struct ethframe frame;
    memcpy(frame.h_dest,   dhaddr, 6);
    memcpy(frame.h_source, shaddr, 6);
    frame.h_proto = htons(proto);
    memcpy(frame.data, data, len);

    syscall(0, (void *)frame_handler, 1, (void *)&frame, len + ETH_HEADER_SIZE);
}

void sendip(const char *data, int len, char proto,
            const char *saddr, const char *daddr,
            const char *shaddr, const char *dhaddr) {
    char buff[MTU];
    struct iphdr *packet = (struct iphdr *)buff;

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
    struct udphdr *packet = (struct udphdr *)buff;
    packet->source = htons(sport);
    packet->dest   = htons(dport);
    packet->len    = htons(UDP_HEADER_SIZE + len);

    memcpy(buff + UDP_HEADER_SIZE, data, len);
    
    sendip(buff, UDP_HEADER_SIZE + len, UDP_PROTO, saddr, daddr, shaddr, dhaddr);
}

void dhcpdiscover(char *shaddr) {
    char buff[DHCP_HEADER_SIZE + 32];
    struct dhcphdr *packet = (struct dhcphdr *)buff;
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

void recvframe(char *buff, uint32_t *len, uint32_t retry)
{
    printf("[U-mode] Receive frame\n");
    syscall(0, (void *)frame_handler, 0, buff, len, retry);
}

int main(int argc, char **argv)
{
    printf("[U-mode] Enter into main function\n");
    char buff[1500];
    uint32_t len;

    dhcpdiscover(src_addrll);
    do {
        recvframe(buff, &len, 20000);
    } while (memcmp(buff, src_addrll, 6));
    printf("Package size: %d\n", len);
    for (int i = 0; i < len; ++i) {
        printf("%02x ", buff[i]);
        if (i % 16 == 15) puts("");
    }
    return 0;
}

void plic_init() {
    // Set UART and SPI interrupt priority
    PLIC_RG_PRIOR_32P[1] = 1;
    PLIC_RG_PRIOR_32P[2] = 1;
    PLIC_RG_PRIOR_32P[3] = 1;

    // Set interrupt 1~31 enable for S-mode
    PLIC_RG_ENABLE_32P[0x20] = ~1;

}

void irq_init() {
    asm volatile ("csrs mstatus, %[rs]"::[rs] "r" (MSTATUS_SPIE));
    asm volatile ("csrs mideleg, %[rs]"::[rs] "r" (MIP_SEIP));
    asm volatile ("csrs mie, %[rs]"::[rs] "r" (MIP_SEIP));
}

void eth_init()
{
    *ETH_RG_TXCTRL_32P = 2 << 16 | 1;
    *ETH_RG_RXCTRL_32P = 2 << 16 | 1;
    *ETH_RG_IE_32P     = 1 << 1;
}

void eth_test(void)
{ 
    /* TM_PRINT="Ethernet test begin" */
    eth_init();
    irq_init();
    plic_init();

    char data[100] = {"Hello World~ RISC-V~"};
    char saddr[] = {0, 0, 0, 0};
    char daddr[] = {255, 255, 255, 255};
    char buff[1500];
    int len;
}

void irq_handler() {
    int irq_id = PERI_PA2KVA(PLIC_RG_INTID, uint32_t)[0x400];
    printk("[S-mode] Receive external interrupt ID: %d\n", irq_id);
    *PERI_PA2KVA(ETH_RG_IE, uint32_t) = 0;
    PERI_PA2KVA(PLIC_RG_INTID, uint32_t)[0x400] = 0;
}

void *frame_handler(trapframe_t *ft) {
    char buff[1500];
    uint32_t *ptr = (uint32_t *)buff;
    bool is_send = ft->a2;
    uint32_t len;
    printk("[S-mode] frame_handler: %s\n", is_send ? "send" : "receive");

    if (is_send) {
        len = (uint32_t)ft->a4;
        copy_from_user(buff, (void *)ft->a3, len);
        while (*PERI_PA2KVA(ETH_RG_RESET, uint32_t) >> 31);
        *PERI_PA2KVA(ETH_RG_TXLEN, uint32_t) = len;

        for (int i = *PERI_PA2KVA(ETH_RG_TXLEN, uint32_t); i > 0; i -= 4)
            *PERI_PA2KVA(ETH_RG_TXFIFO, uint32_t) = *ptr++;
    }
    else {
        uint32_t retry = (uint32_t)ft->a5;
        while (!(len = *PERI_PA2KVA(ETH_RG_RXLEN, uint32_t)) && retry--);
        copy_to_user((void *)ft->a4, &len, sizeof(uint32_t));
        if (!len) return ft;
        for (int i = 0; i < len; i += 4)
            *ptr++ = *PERI_PA2KVA(ETH_RG_RXFIFO, uint32_t);

        *PERI_PA2KVA(ETH_RG_RXDIS, uint32_t) = 1;
        copy_to_user((void *)ft->a3, buff, len);
    }

    return ft;
}
