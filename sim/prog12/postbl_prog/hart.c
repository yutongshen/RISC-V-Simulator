#include <string.h>
#include "riscv_def.h"
#include "hart.h"
#include "mtrap.h"
#include "fdt.h"

extern void fdt_scan(fdt_header *header, fdt_proc *proc);
extern void *get_fdt_address(fdt_scan_prop *prop);
extern int printm(const char* fmt, ...);
extern void sysexit(uint16_t code);

uint32_t hart_phandles[MAX_HARTS];
uint32_t hart_mask = 0;

void hls_init(uint32_t id)
{
    hls_t *hls = HLS_SELF;
    memset(hls, 0, sizeof(hls_t));
}

typedef struct {
    fdt_scan_node *cpu;
    fdt_scan_node *intc;
    int32_t hart_id;
    uint32_t phandle;
    uint32_t int_cells;
} hart_scan;

void hart_open(fdt_scan_node *node, void *buffer)
{
    hart_scan *scan = buffer;

    if (!scan->cpu) scan->hart_id = -1;

    scan->int_cells = 0;
    scan->phandle = 0;
}

void hart_prop(fdt_scan_prop *prop, void *buffer)
{
    hart_scan *scan = buffer;
    
    if (!strcmp(prop->name, "device_type") && !strcmp((const char *) prop->value, "cpu"))
    {
        scan->cpu = prop->node;
    }
    else if (!strcmp(prop->name, "reg"))
    {
        scan->hart_id = (uintptr_t) get_fdt_address(prop);
    }
    else if (!strcmp(prop->name, "interrupt-controller"))
    {
        scan->intc = prop->node;
    }
    else if (!strcmp(prop->name, "phandle"))
    {
        scan->phandle = byte_reverse(*prop->value);
    }
    else if (!strcmp(prop->name, "#interrupt-cells"))
    {
        scan->int_cells = byte_reverse(*prop->value);
    }
}

void hart_done(fdt_scan_node *node, void *buffer)
{
    hart_scan *scan = buffer;

    if (scan->cpu == node) assert(scan->hart_id >= 0);
    if (scan->intc == node && scan->cpu)
    {
        assert (scan->phandle > 0);
        assert (scan->int_cells == 1);

        if (scan->hart_id < MAX_HARTS)
        {
            hart_phandles[scan->hart_id] = scan->phandle;
            hart_mask |= 1 << scan->hart_id;
            hls_init(scan->hart_id);
            /* TM_PRINT="Find CPU@%ld", (uint64_t) scan->hart_id */
        }
    }

}

void hart_close(fdt_scan_node *node, void *buffer)
{
    hart_scan *scan = (hart_scan *) buffer;
    if (scan->cpu == node) scan->cpu = 0;
    if (scan->intc == node) scan->intc = 0;
}

void query_hart(void *dtb)
{
    fdt_proc proc;
    hart_scan scan;
    memset(&proc, 0, sizeof(proc));
    proc.open = &hart_open;
    proc.prop = &hart_prop;
    proc.done = &hart_done;
    proc.close = &hart_close;
    proc.buffer = &scan;
    fdt_scan(dtb, &proc);
}

volatile uintptr_t *mtime;

typedef struct {
    uint32_t find;
    uint32_t *addr;
    uint32_t *ints;
    uint32_t len;
} clint_scan;

void clint_open(fdt_scan_node *node, void *buffer)
{
    clint_scan *scan = buffer;

    scan->find = 0;
    scan->addr = 0;
    scan->ints = 0;
    scan->len  = 0;
}

void clint_prop(fdt_scan_prop *prop, void *buffer)
{
    clint_scan *scan = buffer;
    
    if (!strcmp(prop->name, "compatible") && !strcmp((const char *) prop->value, "riscv,clint0"))
    {
        scan->find = 1;
    }
    else if (!strcmp(prop->name, "reg"))
    {
        scan->addr = get_fdt_address(prop);
    }
    else if (!strcmp(prop->name, "interrupts-extended"))
    {
        scan->ints = prop->value;
        scan->len  = prop->len;
    }
}

void clint_done(fdt_scan_node *node, void *buffer)
{
    clint_scan *scan = buffer;

    if (scan->find)
    {
        assert(scan->addr);
        assert(!(scan->len & 0xf)); // Must 4 (value) * 4 (bytes/value) * n (hart)
        scan->find = 0;
        
        /* TM_PRINT="Find CLINT@%lx", (uint64_t) scan->addr */
        mtime = (uintptr_t *) (scan->addr + RG_CLINT_TIME);

        uint32_t n = scan->len >> 4;

        for (uint32_t i = 0; i < n; ++i)
        {
            assert(scan->ints[i << 2] == scan->ints[(i << 2) + 2]);
            assert(byte_reverse(scan->ints[(i << 2) + 1]) == MTIP_BIT ||
                   byte_reverse(scan->ints[(i << 2) + 1]) == MSIP_BIT);
            assert(byte_reverse(scan->ints[(i << 2) + 1] ^ scan->ints[(i << 2) + 3]) == (MTIP_BIT ^ MSIP_BIT));

            uint32_t phandle = byte_reverse(scan->ints[i << 2]);
            for (uint32_t hart_id = 0; hart_id < MAX_HARTS; ++hart_id)            
            {
                if (hart_phandles[hart_id] == phandle)
                {
                    /* TM_PRINT="Routing CPU%d IPC", hart_id */
                    hls_t *hls = HLS(hart_id);
                    hls->ipi = scan->addr + RG_CLINT_MSIP + i;
                    hls->mtimecmp = (uintptr_t *) (scan->addr + RG_CLINT_TIMECMP + (i << 1));
                    break;
                }
            }
        }
    }

}

void query_clint(void *dtb)
{
    fdt_proc proc;
    clint_scan scan;
    memset(&proc, 0, sizeof(proc));
    proc.open = &clint_open;
    proc.prop = &clint_prop;
    proc.done = &clint_done;
    proc.buffer = &scan;
    fdt_scan(dtb, &proc);
}

volatile uint32_t *ints_priority;

typedef struct {
    uint32_t find;
    uint32_t *addr;
    uint32_t *ints;
    uint32_t len;
} plic_scan;

void plic_open(fdt_scan_node *node, void *buffer)
{
    plic_scan *scan = buffer;

    scan->find = 0;
    scan->addr = 0;
    scan->ints = 0;
    scan->len  = 0;
}

void plic_prop(fdt_scan_prop *prop, void *buffer)
{
    plic_scan *scan = buffer;
    
    if (!strcmp(prop->name, "compatible") && !strcmp((const char *) prop->value, "riscv,plic0"))
    {
        scan->find |= 1;
    }
    else if (!strcmp(prop->name, "interrupt-controller"))
    {
        scan->find |= 2;
    }
    else if (!strcmp(prop->name, "reg"))
    {
        scan->addr = get_fdt_address(prop);
    }
    else if (!strcmp(prop->name, "interrupts-extended"))
    {
        scan->ints = prop->value;
        scan->len  = prop->len;
    }
}

void plic_done(fdt_scan_node *node, void *buffer)
{
    plic_scan *scan = buffer;

    if (scan->find == 0x3)
    {
        assert(scan->addr);
        assert(!(scan->len & 0x7)); // Must 2 (value) * 4 (bytes/value) * n (M-mode & S-mode)
        scan->find = 0;
        
        /* TM_PRINT="Find PLIC@%lx", (uint64_t) scan->addr */
        ints_priority = scan->addr + RG_PLIC_PRIOR;

        uint32_t n = scan->len >> 3;

        for (uint32_t i = 0; i < n; ++i)
        {
            uint32_t phandle = byte_reverse(scan->ints[i << 1]);
            uint32_t ints_type = byte_reverse(scan->ints[(i << 1) + 1]);

            for (uint32_t hart_id = 0; hart_id < MAX_HARTS; ++hart_id)            
            {
                if (hart_phandles[hart_id] == phandle)
                {
                    hls_t *hls = HLS(hart_id);
                    if (ints_type == MEIP_BIT)
                    {
                        /* TM_PRINT="Routing CPU%d MEIP", hart_id */
                        hls->plic_mth = (void *) (scan->addr + RG_PLIC_PRIOR_TH + (i << 10));
                        hls->plic_mie = scan->addr + RG_PLIC_ENABLE + (i << 5);
                    }
                    else if (ints_type == SEIP_BIT)
                    {
                        /* TM_PRINT="Routing CPU%d SEIP", hart_id */
                        hls->plic_sth = (void *) (scan->addr + RG_PLIC_PRIOR_TH + (i << 10));
                        hls->plic_sie = scan->addr + RG_PLIC_ENABLE + (i << 5);
                    }
                    break;
                }
            }
        }
    }

}

void query_plic(void *dtb)
{
    fdt_proc proc;
    plic_scan scan;
    memset(&proc, 0, sizeof(proc));
    proc.open = &plic_open;
    proc.prop = &plic_prop;
    proc.done = &plic_done;
    proc.buffer = &scan;
    fdt_scan(dtb, &proc);
}
