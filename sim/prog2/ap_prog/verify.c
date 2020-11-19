#include <stdint.h>
#include "mmap_soc.h"

static volatile uint32_t *uart_txfifo = (volatile uint32_t *) UART_RG_TXFIFO;
static volatile uint32_t *uart_txctrl = (volatile uint32_t *) UART_RG_TXCTRL;

void cputchar(uint8_t ch)
{
    int32_t r;
    do {
        __asm__ __volatile__("amoor.w %0, %2, %1\n"
                             : "=r"(r), "+A"(*uart_txfifo)
                             : "r"(ch));
    } while (r < 0);
}

void cputs(const uint8_t *str)
{
    while (*str)
        cputchar(*(str++));
}

void print_hex(uint32_t num)
{
    uint8_t d, i;
    for (i = 0; i < 8; num <<= 4, ++i) {
        d = ((num & 0xf0000000) >> 28);
        if (d < 10)
            cputchar(d + 0x30);
        else
            cputchar(d - 10 + 0x61);
    }
}

void print_mismatch(uint32_t *test, uint32_t *expect)
{
    cputs("MEM[");
    print_hex((uint64_t) test);
    cputs("] = ");
    print_hex(*test);
    cputs(", expect ");
    print_hex(*expect);
    cputchar('\n');
}

void print_match(uint32_t *test)
{
    cputs("MEM[");
    print_hex((uint64_t) test);
    cputs("] = ");
    print_hex(*test);
    cputs(", pass!");
    cputchar('\n');
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
        cputs("============== FAIL ==============\n");
        cputs("There are some error.\n");
    } else {
        cputs("============ ALL PASS ============\n");
    }

    return err;
}
