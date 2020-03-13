#include <stdint.h>

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

#define SYS_WRITE 0x40

static void syscall(uint64_t sys_id, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    volatile uint64_t tohost_msg[4];
    tohost_msg[0] = sys_id;
    tohost_msg[1] = arg0;
    tohost_msg[2] = arg1;
    tohost_msg[3] = arg2;
    
    tohost = (uint64_t) tohost_msg;
    while (!fromhost);
    
    fromhost = 0;
}

int putchar(int ch) {
    static char buff[64];
    static uint64_t len = 0;
    buff[len++] = ch;

    if (ch == '\0' || len == sizeof(buff)) {
        syscall(SYS_WRITE, 1, (uint64_t) buff, len);
        len = 0;
    }

    return 0;
}
