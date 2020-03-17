#include "util.h"
#include <stdarg.h>

#define SYS_WRITE 0x40

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

void *memset(void *ptr, int val, size_t len)
{
    uint64_t *dword_ptr = ptr;
    uint64_t write_dword = (char) val;
    write_dword |= write_dword << 8;
    write_dword |= write_dword << 16;
    write_dword |= write_dword << 32;

    while (len >= 8) {
        *dword_ptr++ = write_dword;
        len -= 8;
    }

    char *byte_ptr = (char *) dword_ptr;
    while (len--) {
        *byte_ptr++ = val;
    }

    return ptr;
}

void *memcpy(void *ptr, void *src, size_t len)
{
    uint64_t *dword_ptr = ptr;
    uint64_t *dword_src = src;

    while (len >= 8) {
        *dword_ptr++ = *dword_src++;
        len -= 8;
    }

    char *byte_ptr = (char *) dword_ptr;
    char *byte_src = (char *) dword_src;
    while (len--) {
        *byte_ptr++ = *byte_src++;
    }

    return ptr;
}

static void syscall(uint64_t sys_id,
                    uint64_t arg0,
                    uint64_t arg1,
                    uint64_t arg2)
{
    volatile uint64_t tohost_msg[4];
    tohost_msg[0] = sys_id;
    tohost_msg[1] = arg0;
    tohost_msg[2] = arg1;
    tohost_msg[3] = arg2;

    tohost = (uint64_t) tohost_msg & ((1UL << 21) - 1UL) | 0x80000000;
    while (!fromhost)
        ;

    fromhost = 0;
}

int putchar(int ch)
{
    static char buff[64];
    static uint64_t len = 0;
    buff[len++] = ch;

    if (ch == '\n' || len == sizeof(buff)) {
        syscall(SYS_WRITE, 1,
                (uint64_t) buff & ((1UL << 21) - 1UL) | 0x80000000, len);
        len = 0;
    }

    return 0;
}

int puts(const char *s)
{
    int res = 0;
    while (*s) {
        putchar(*(s++));
        ++res;
    }

    return res;
}

char *itoa(uint64_t value, int base, int min_len, char fill_char)
{
    static char digitals[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    static char str[64];
    char *ptr = str + 63;
    int tmp;
    *ptr = 0;
    do {
        if (!value)
            *--ptr = fill_char;
        else
            *--ptr = digitals[value % base];
        value /= base;
    } while (--min_len > 0 || value);

    return ptr;
}

int printf(const char *format, ...)
{
    const char *ptr = format;
    int res = 0;

    va_list va;
    va_start(va, format);

    while (*ptr) {
        if (*ptr == '%') {
            int min_len = 0;
            char fill_char = 0;
        loop1:
            switch (*(++ptr)) {
            case 'c':
                assert(!(min_len || fill_char));
                ++res;
                putchar(va_arg(va, int));
                break;
            case 's':
                assert(!(min_len || fill_char));
                res += puts(va_arg(va, const char *));
                break;
            case 'x': {
                assert(!(!fill_char ^ !min_len));
                uint64_t n = va_arg(va, uint64_t);
                res += puts(itoa(n, 16, min_len, fill_char));
            } break;
            case 'd': {
                assert(!(!fill_char ^ !min_len));
                int64_t n = va_arg(va, int64_t);
                if (n < 0) {
                    ++res;
                    putchar('-');
                    n = -n;
                }
                res += puts(itoa(n, 10, min_len, fill_char));
            } break;
            case '%':
                ++res;
                putchar('%');
                break;
            case '1':
                assert(fill_char);
                min_len *= 10;
                min_len += 1;
                goto loop1;
            case '2':
                assert(fill_char);
                min_len *= 10;
                min_len += 2;
                goto loop1;
            case '3':
                assert(fill_char);
                min_len *= 10;
                min_len += 3;
                goto loop1;
            case '4':
                assert(fill_char);
                min_len *= 10;
                min_len += 4;
                goto loop1;
            case '5':
                assert(fill_char);
                min_len *= 10;
                min_len += 5;
                goto loop1;
            case '6':
                assert(fill_char);
                min_len *= 10;
                min_len += 6;
                goto loop1;
            case '7':
                assert(fill_char);
                min_len *= 10;
                min_len += 7;
                goto loop1;
            case '8':
                assert(fill_char);
                min_len *= 10;
                min_len += 8;
                goto loop1;
            case '9':
                assert(fill_char);
                min_len *= 10;
                min_len += 9;
                goto loop1;
            default:
                assert(!min_len);
                fill_char = *ptr;
                goto loop1;
            }
        } else {
            ++res;
            putchar(*ptr);
        }
        ++ptr;
    }
    return res;
}
