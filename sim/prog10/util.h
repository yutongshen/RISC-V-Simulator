#ifndef __UTIL__
#define __UTIL__
#include <stdint.h>
#include <stddef.h>

#define assert(x)                                                     \
    do {                                                              \
        if (!x) {                                                     \
            printf("Failed assertion '%s' at line %d of '%s'.\n", #x, \
                   __LINE__, __FILE__);                               \
            tohost = 3;                                               \
        }                                                             \
    } while (0)


void *memset(void *ptr, int val, size_t len);
void *memcpy(void *ptr, void *src, size_t len);
static void syscall(uint64_t sys_id,
                    uint64_t arg0,
                    uint64_t arg1,
                    uint64_t arg2);
int putchar(int ch);
int puts(const char *s);
int printf(const char *format, ...);
#endif
