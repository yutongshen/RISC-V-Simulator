#include <string.h>
#include <stddef.h>
#include "finisher.h"
#include "fdt.h"

volatile uint32_t *finisher = 0;

extern void __wfi(void);
extern void fdt_scan(fdt_header *header, fdt_proc *proc);
extern void *get_fdt_address(fdt_scan_prop *prop);

void finisher_open(fdt_scan_node *node, void *buffer)
{
    finisher_scan *scan = buffer;
    
    scan->find = 0;
    scan->addr = NULL;
}

void finisher_prop(fdt_scan_prop *prop, void *buffer)
{
    finisher_scan *scan = buffer;
    
    if (!strcmp(prop->name, "compatible") && !strcmp((const char *) prop->value, "sifive,test0"))
    {
        scan->find = 1;
    }
    else if (!strcmp(prop->name, "reg"))
    {
        scan->addr = (uint32_t *) get_fdt_address(prop);
    }
}

void finisher_done(fdt_scan_node *node, void *buffer)
{
    finisher_scan *scan = buffer;

    if (scan->find && !finisher && scan->addr)
    {
        finisher = (uint32_t *) scan->addr;
        /* TM_PRINT="Find finisher@0x%lx", (uint64_t) finisher */
    }
}

void query_finisher(void *dtb)
{
    fdt_proc proc;
    finisher_scan scan;
    memset(&proc, 0, sizeof(proc));
    proc.open = &finisher_open;
    proc.prop = &finisher_prop;
    proc.done = &finisher_done;
    proc.buffer = &scan;
    fdt_scan(dtb, &proc);
}

void finisher_exit(uint16_t code)
{
    if (finisher) *finisher = (code << 16) | (code ? FINISHER_FAIL : FINISHER_PASS);

    while (1) __wfi();
}
