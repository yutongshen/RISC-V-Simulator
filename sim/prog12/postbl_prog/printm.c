#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#define isdigtal(x) ((x) >= '0' && (x) <= '9')
#define FLAG_NONE      (0x0)
#define FLAG_LEFT      (0x1 << 0)
#define FLAG_PLUS      (0x1 << 1)
#define FLAG_SPACE     (0x1 << 2)
#define FLAG_SPACIAL   (0x1 << 3)
#define FLAG_ZEROPAD   (0x1 << 4)
#define FLAG_UNSIGN    (0x1 << 5)
#define FLAG_UPPERCASE (0x1 << 6)

extern uint64_t mcall_putchar(uint8_t ch);

void putstring(const char *str)
{
    while (*str) mcall_putchar(*str++);
}

#define BUFLEN 128
void int64tostr(int64_t value, char **str, uint32_t base, uint32_t flags, int32_t field_width)
{
    char buf[BUFLEN];
    char sign = !(flags & FLAG_UNSIGN) && value < 0;
    char *ptr = &buf[BUFLEN - 1];
    char hex_dig_start = (flags & FLAG_UPPERCASE) ? 'A' : 'a';
    char filler = (flags & FLAG_ZEROPAD) ? '0' : ' ';

    value = value ^ (-sign);
    *ptr = 0;

    do
    {
        int dig = value % base;
        *--ptr = dig < 10 ? '0' + dig : hex_dig_start + dig - 10;
    } while (value /= base);


    uint32_t len = strlen(ptr);

    if (sign)                    *(*str)++ = '-', --len;
    else if (flags & FLAG_PLUS)  *(*str)++ = '+', --len;
    else if (flags & FLAG_SPACE) *(*str)++ = ' ', --len;

    if (flags & FLAG_SPACIAL)
    {
        if (base == 16)
        {
            (*str)[0] = '0';
            (*str)[1] = 'x';
            (*str) += 2;
            len += 2;
        }
        else if (base == 8)
        {
            *(*str)++ = '0';
            ++len;
        }
    }

    field_width -= len;
    if (!(flags & FLAG_LEFT))
    {
        while (field_width-- > 0) *(*str)++ = filler;
    }
    while (*ptr) *(*str)++ = *ptr++;
    while (field_width-- > 0) *(*str)++ = ' ';
}


int vsprintf(char* out, const char* fmt, va_list args)
{
    char *str = out;
    char flags;
    char qualifier;
    int  field_width;
    int base;

    for (; *fmt; ++fmt) 
    {
        if (*fmt != '%')
        {
            *str++ = *fmt;
            continue;
        }

        flags = FLAG_NONE;
        while (1)
        {
            switch (*++fmt)
            {
                case '-': flags |= FLAG_LEFT; break;
                case '+': flags |= FLAG_PLUS; break;
                case ' ': flags |= FLAG_SPACE; break;
                case '#': flags |= FLAG_SPACIAL; break;
                case '0': flags |= FLAG_ZEROPAD; break;
                default: goto flag_done;
            }
        }

        flag_done:

        field_width = -1;
        if (isdigtal(*fmt))
        {
            field_width = *fmt++ - '0';
            while (isdigtal(*fmt))
            {
                field_width = field_width * 10 + *fmt++ - '0';
            }
        }
        else if (*fmt == '*')
        {
            field_width = va_arg(args, int32_t);
            if (field_width & 0x80000000)
            {
                field_width = -field_width;
                flags |= FLAG_LEFT;
            }
        }

        qualifier = 0;
        if (*fmt == 'l' || *fmt == 'L' || *fmt == 'h')
        {
            qualifier = *fmt++;
        }

        switch (*fmt) {
            case '%':
                *str++ = '%';
                continue;
            case 's':
                {
                    const char *s = va_arg(args, const char *);
                    uint32_t len = strlen(s);
                    
                    field_width -= len;
                    if (!(flags & FLAG_LEFT))
                    {
                        while (field_width-- > 0) *str++ = ' ';
                    }
                    while (*s) *str++ = *s++;
                    while (field_width-- > 0) *str++ = ' ';
                }
                continue;
            case 'n':
                {
                    if (qualifier == 'l') {
                        int64_t *ip = va_arg(args, int64_t *);
                        *ip = (str - out);
                    } else {
                        int32_t *ip = va_arg(args, int32_t *);
                        *ip = (str - out);
                    }
                }
                continue;
            case 'p':
                flags |= FLAG_SPACIAL;
                if (field_width == -1) {
                    field_width = 2 * sizeof(void *);
                    flags |= FLAG_ZEROPAD;
                }
                int64tostr((uint64_t) va_arg(args, void *), &str, 16, flags, field_width);
                continue;
            case 'c':
                *str++ = (char) va_arg(args, int32_t);
                continue;
            case 'X':
                flags |= FLAG_UPPERCASE;
            case 'x':
                base = 16;
                break;
            case 'o':
                base = 8;
                break;
            case 'u':
                flags |= FLAG_UNSIGN;
            case 'd':
            case 'i':
                base = 10;
                break;
            default:
                *str++ = '%';
                if (*fmt) *str++ = *fmt;
                else --fmt;
                continue;
        }
        if (qualifier == 'l')
            int64tostr(va_arg(args, int64_t), &str, base, flags, field_width);
        else if (qualifier == 'h')
            int64tostr((int16_t) va_arg(args, int32_t), &str, base, flags, field_width);
        else
            int64tostr(va_arg(args, int32_t), &str, base, flags, field_width);
    }
    *str = '\0';
    return str - out;
}

int sprintf(char* out, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int res = vsprintf(out, fmt, args);
    va_end(args);
    return res;
}

int vprintm(const char* fmt, va_list args)
{
    char buf[BUFLEN];
    int res = vsprintf(buf, fmt, args);
    putstring(buf);
    return res;
}

int printm(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int res = vprintm(fmt, args);
    va_end(args);
    return res;
}
