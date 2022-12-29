#include "util.h"
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include "mmap_soc.h"
#include "riscv_def.h"

size_t strlen(const char *str) {
    size_t res = 0;
    while (*str++) res++;
    return res;
}

void *memset(void * ptr, int val, size_t len)
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

void *memcpy(void *restrict ptr, const void *restrict src, size_t len)
{
    uint64_t *dword_ptr = ptr;
    const uint64_t *dword_src = src;

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

int memcmp(const void *a, const void *b, size_t len) {
    while (len--) {
        if (*(const char *)a != *(const char *)b)
            return *(const char *)a - *(const char *)b;
        ++a, ++b;
    }
    return 0;
}

uint64_t syscall(bool tohost, ...)
{
    if (tohost)
        asm volatile(
            "slli a1, a1, 48;"
            "or a1, a1, a2;"
            "ecall;" ::: "a0");
    else
        asm volatile(
            "ecall;" ::: "a0");
    return 0;
}

void abort(void)
{
    syscall(1, SYSCALL_SYS, 3);
    while (1);
}

int putchar(int ch)
{
    syscall(1, SYSCALL_PUTCHAR, ch);

    return 0;
}

int _puts(const char *s)
{
    int res = 0;
    while (*s) {
        putchar(*(s++));
        ++res;
    }

    return res;
}

int puts(const char *s)
{
    int res = 1;
    res += _puts(s);
    putchar('\n');

    return res;
}

int fclass(double x)
{
    int cls;
    asm volatile("fclass.d %0, %1;" : "=r"(cls) : "f"(x));
    return cls;
}

double log_2(double x)
{
    int cls = fclass(x);
    if (!(cls & 0x60))
        return NAN;
    union {
        double d;
        int64_t i;
    } _x;
    double res = 0;
    _x.d = x;
    if (cls == 0x20) {
        res -= 1022 + 52;
        _x.d = (double) _x.i;
    }
    res += (double) (((_x.i >> 52) & 0x7ff) - 0x400L);
    // if (!(_x.i & ((1UL << 52) - 1)))
    //    return res + 1;
    _x.i &= ~(0x7ffUL << 52);
    _x.i |= 0x3ffUL << 52;
    res += ((-0.3358287811) * _x.d + 2.) * _x.d - 0.65871759316667;
    return res;
}

double log10(double x)
{
    if (!(fclass(x) & 0x60))
        return NAN;
    return log_2(x) / 3.3219280948873626;
}

double fabs(double x)
{
    union {
        double d;
        int64_t i;
    } _x;
    _x.d = x;
    _x.i &= -1UL >> 1;
    return _x.d;
}

char *ftoa(double value)
{
    static double base[] = {1e+1,  1e+2,  1e+4,   1e+8,  1e+16,
                            1e+32, 1e+64, 1e+128, 1e+256};
    static char str[32];
    uint16_t cls = fclass(value);
    if (cls & (1 << 0))
        return "-inf";
    if (cls & (1 << 7))
        return "inf";
    if (cls & (1 << 8))
        return "snan";
    if (cls & (1 << 9))
        return "qnan";

    char *ptr = str;
    int64_t exp = log10(fabs(value));
    if (exp < 6 && exp >= -2) {
        uint64_t l;
        char *tmp;
        ++ptr;
        *str = ' ';
        if (value < 0.) {
            *ptr++ = '-';
            value = -value;
        }
        l = value;
        tmp = itoa(l, 10, 0, '0');
        while (*tmp)
            *ptr++ = *tmp++;
        *ptr++ = '.';
        value -= l;
        l = (value * 1e+7);
        tmp = itoa(l, 10, 7, '0');
        while (*tmp)
            *ptr++ = *tmp++;
        if (*--ptr >= '5') {
            *ptr = 0;
            while (1) {
                ++*--ptr;
                if (*(ptr) <= '9')
                    break;
                *ptr = '0';
                if (*(ptr - 1) == '.')
                    --ptr;
                else if (*(ptr - 1) == '-') {
                    *(ptr - 2) = *(ptr - 1);
                    *(ptr - 1) = '1';
                    return str;
                } else if (*(ptr - 1) == ' ') {
                    *(ptr - 1) = '1';
                    return str;
                }
            }
        } else
            *ptr = 0;
        return str + 1;
    } else {
        double _base = 1.;
        char *tmp;
        if (exp >= 0) {
            for (int i = 0; exp >> i; ++i)
                if ((exp >> i) & 1)
                    _base *= base[i];
            tmp = ftoa(value / _base);
            while (*tmp)
                *ptr++ = *tmp++;
            *ptr++ = 'e';
            *ptr++ = '+';
        } else {
            exp = -exp + 1;
            for (int i = 0; exp >> i; ++i)
                if ((exp >> i) & 1)
                    _base *= base[i];
            tmp = ftoa(value * _base);
            while (*tmp)
                *ptr++ = *tmp++;
            *ptr++ = 'e';
            *ptr++ = '-';
        }
        tmp = itoa(exp, 10, 0, '0');
        while (*tmp)
            *ptr++ = *tmp++;
        return str;
    }
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
                res += _puts(va_arg(va, const char *));
                break;
            case 'p': {
                assert(!(!fill_char ^ !min_len));
                uint64_t n = va_arg(va, uint64_t);
                res += _puts("0x");
                res += _puts(itoa(n, 16, min_len, min_len ? fill_char : '0'));
            } break;
            case 'x': {
                assert(!(!fill_char ^ !min_len));
                uint64_t n = va_arg(va, uint64_t);
                res += _puts(itoa(n, 16, min_len, min_len ? fill_char : '0'));
            } break;
            case 'd': {
                assert(!(!fill_char ^ !min_len));
                int64_t n = va_arg(va, int64_t);
                if (n < 0) {
                    ++res;
                    putchar('-');
                    n = -n;
                }
                res += _puts(itoa(n, 10, min_len, min_len ? fill_char : '0'));
            } break;
            case 'f': {
                assert(!(!fill_char ^ !min_len));
                double n = va_arg(va, double);
                res += _puts(ftoa(n));
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

