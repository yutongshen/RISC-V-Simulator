#include <stdint.h>
#include "mmap_soc.h"

static volatile uint32_t *uart_txfifo = (volatile uint32_t *) UART_RG_TXFIFO;
static volatile uint32_t *uart_txctrl = (volatile uint32_t *) UART_RG_TXCTRL;

void putchar(uint8_t ch)
{
    int32_t r;
    do {
        __asm__ __volatile__("amoor.w %0, %2, %1\n"
                             : "=r"(r), "+A"(*uart_txfifo)
                             : "r"(ch));
    } while (r < 0);
}

void puts(const uint8_t *str)
{
    while (*str)
        putchar(*(str++));
}

void print_hex(uint32_t num)
{
    uint8_t d, i;
    for (i = 0; i < 8; num <<= 4, ++i) {
        d = ((num & 0xf0000000) >> 28);
        if (d < 10)
            putchar(d + 0x30);
        else
            putchar(d - 10 + 0x61);
    }
}

void print_mismatch(uint32_t *test, uint32_t *expect)
{
    puts("MEM[");
    print_hex((uint32_t) test);
    puts("] = ");
    print_hex(*test);
    puts(", expect ");
    print_hex(*expect);
    putchar('\n');
}

void print_match(uint32_t *test)
{
    puts("MEM[");
    print_hex((uint32_t) test);
    puts("] = ");
    print_hex(*test);
    puts(", pass!");
    putchar('\n');
}

uint32_t ans_cmp()
{
    extern uint32_t _golden_start[];
    extern uint32_t _golden_end[];
    extern uint32_t _test_start[];

    uint32_t *test = _test_start;
    uint32_t *golden = _golden_start;
    uint32_t err = 0;

    *uart_txctrl = 0x1;

    for (; golden < _golden_end; ++golden, ++test) {
        if (*test == *golden) {
            print_match(test);
        } else {
            print_mismatch(test, golden);
            ++err;
        }
    }

    if (err) {
        puts("============== FAIL ==============\n");
        puts("There are some error.\n");
    } else {
        puts("============ ALL PASS ============\n");
    }

    return err;
}
