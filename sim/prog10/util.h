#ifndef __UTIL__
#define __UTIL__
#include <stdint.h>

static void syscall(uint64_t sys_id,
                    uint64_t arg0,
                    uint64_t arg1,
                    uint64_t arg2);
int putchar(int ch);
int puts(const char *s);
int printf(const char *format, ...);
#endif
