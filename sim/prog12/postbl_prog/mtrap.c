#include <stdint.h>


extern void finisher_exit(uint16_t code);
extern void uart_putchar(uint8_t ch);

uint64_t mcall_putchar(uint8_t ch)
{
    uart_putchar(ch);
    return 0;
}

void sysexit(int16_t code)
{
    finisher_exit(code);
}
