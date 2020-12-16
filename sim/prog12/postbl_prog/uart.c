#include <string.h>
#include <stddef.h>
#include "uart.h"
#include "fdt.h"

volatile uint32_t *uart = 0;

extern void fdt_scan(fdt_header *header, fdt_proc *proc);
extern void *get_fdt_address(fdt_scan_prop *prop);

void uart_open(fdt_scan_node *node, void *buffer)
{
    uart_scan *scan = buffer;
    
    scan->find = 0;
    scan->addr = NULL;
}

void uart_prop(fdt_scan_prop *prop, void *buffer)
{
    uart_scan *scan = buffer;
    
    if (!strcmp(prop->name, "compatible") && !strcmp((const char *) prop->value, "sifive,uart0"))
    {
        scan->find = 1;
    }
    else if (!strcmp(prop->name, "reg"))
    {
        scan->addr = (uint32_t *) get_fdt_address(prop);
    }
}

void uart_done(fdt_scan_node *node, void *buffer)
{
    uart_scan *scan = buffer;

    if (scan->find && !uart && scan->addr)
    {
        uart = (uint32_t *) scan->addr;
        /* TM_PRINT="Find UART@0x%lx", (uint64_t) uart */
        uart[RG_UART_TXCTRL] = UART_TXEN;
        uart[RG_UART_RXCTRL] = UART_RXEN;
    }
}

void query_uart(void *dtb)
{
    fdt_proc proc;
    uart_scan scan;
    memset(&proc, 0, sizeof(proc));
    proc.open = &uart_open;
    proc.prop = &uart_prop;
    proc.done = &uart_done;
    proc.buffer = &scan;
    fdt_scan(dtb, &proc);
}

void uart_putchar(uint8_t ch)
{
    if (!uart) return;

    int32_t r;
    do {
        asm volatile(
            "amoor.w %0, %2, %1\n"
            : "=r" (r), "+A" (uart[RG_UART_TXFIFO])
            : "r" (ch)
        );
    } while (r);
}
