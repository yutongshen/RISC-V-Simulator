#include <string.h>
#include <stddef.h>
#include "mmap_soc.h"
#include "fdt.h"

uint32_t byte_reverse(uint32_t x)
{
    x = ((x & 0x0000ffff) << 16) | ((x & 0xffff0000) >> 16);
    x = ((x & 0x00ff00ff) <<  8) | ((x & 0xff00ff00) >>  8);
    return x;
}

void print(const char *str)
{
    *UART_RG_TXCTRL_32P = 1;
    do { *UART_RG_TXFIFO_32P = *str; } while (*(++str));
    *UART_RG_TXFIFO_32P = '\n';
}

void test_open(fdt_scan_node *node, void *buffer)
{
    /* TM_PRINT="TEST_OPEN:" */
    print(node->name);
}

void test_prop(fdt_scan_prop *prop, void *buffer)
{
    /* TM_PRINT="TEST_PROP:" */
    print(prop->name);
    /* TM_PRINT="VALUE: %d", byte_reverse(*prop->value)*/
    print((const char *) prop->value);
}

void test_done(fdt_scan_node *node, void *buffer)
{
    /* TM_PRINT="TEST_DONE:" */
    print(node->name);
}

const uint32_t *fdt_scan_recursive(fdt_scan_node *node, fdt_proc *proc, const uint32_t *struct_ptr, const char *string, int level)
{
    // /* TM_PRINT="==== FDT SCAN RECURSIVE BEGIN LEVEL %d ====", level */
    fdt_scan_node child;
    fdt_scan_prop prop;
    
    child.parent        = node;
    child.address_cells = 2;
    child.size_cells    = 1;
    prop.node           = node;

    while (1) {
        // /* TM_PRINT="STRUCT = 0x%x", byte_reverse(*struct_ptr) */
        switch (byte_reverse(*struct_ptr)) {
        case FDT_BEGIN_NODE:
            child.name = (const char *) &struct_ptr[1];
            // /* TM_PRINT="FDT_BEGIN_NODE:" */
            // /* TM_PRINT="PTR = %p", (uint64_t) child.name */
            // /* TM_PRINT="NAME_LEN = %d", strlen(child.name) */
            // /* TM_PRINT="NAME:" */
            // print(child.name);
            if (proc->open) proc->open(&child, proc->buffer);
            struct_ptr = fdt_scan_recursive(&child, proc, struct_ptr + 2 + (strlen(child.name) >> 2), string, level + 1);
            break;
        case FDT_END_NODE:
            // /* TM_PRINT="==== FDT SCAN RECURSIVE END LEVEL %d ====", level */
            ++struct_ptr;
            if (proc->done) proc->done(node, proc->buffer);
            return struct_ptr;
        case FDT_PROP:
            prop.len = byte_reverse(struct_ptr[1]);
            prop.name = byte_reverse(struct_ptr[2]) + string;
            prop.value = (uint32_t *) &struct_ptr[3];
            struct_ptr += 3 + ((prop.len + 3) >> 2);
            if (node && !strcmp(prop.name, "#address-cells")) node->address_cells = byte_reverse(*prop.value);
            else if (node && !strcmp(prop.name, "#size-cells")) node->size_cells = byte_reverse(*prop.value);
            if (proc->prop) proc->prop(&prop, proc->buffer);
            // /* TM_PRINT="FDT_PROP:" */
            // /* TM_PRINT="PTR = %p", (uint64_t) prop.name */
            // /* TM_PRINT="NAME_LEN = %d", strlen(prop.name) */
            // /* TM_PRINT="PROP_NAME:" */
            // print(prop.name);
            // /* TM_PRINT="PROP_VALUE: %x", byte_reverse(*(prop.value)) */
            // /* TM_PRINT="PROP_VALUE:" */
            // print((const char *) prop.value);
            break;
        case FDT_NOP:
            ++struct_ptr;
            break;
        case FDT_END:
            // /* TM_PRINT="==== FDT SCAN RECURSIVE END LEVEL %d ====", level */
            ++struct_ptr;
            return struct_ptr;
        default:
            // /* TM_PRINT="==== FDT SCAN RECURSIVE END LEVEL %d ====", level */
            ++struct_ptr;
            return struct_ptr;
        }
    }
}

void fdt_scan(fdt_header *header, fdt_proc *proc)
{
    /* TM_PRINT="FDT SCAN BEGIN" */
    /* TM_PRINT="MAGIC   = 0x%x", byte_reverse(header->magic) */
    /* TM_PRINT="VERSION = 0x%x", byte_reverse(header->version) */
    if (byte_reverse(header->magic) != FDT_MAGIC) return;

    fdt_scan_recursive(
        NULL,
        proc,
        (const uint32_t *) ((void *) header + byte_reverse(header->off_dt_struct)),
        (const char *) header + byte_reverse(header->off_dt_strings),
        0
    );
    /* TM_PRINT="FDT SCAN END" */
}

void *get_fdt_address(fdt_scan_prop *prop)
{
    uint64_t res = 0;
    uint32_t cells = prop->node->parent->address_cells;
    uint32_t *value = prop->value;
    while (cells--)
    {
        res = (res << 32) | byte_reverse(*value++);
    }
    return (void *) res;
}
