#ifndef __FDT__
#define __FDT__

#include <stdint.h>

#define FDT_MAGIC      0xd00dfeed
#define FDT_BEGIN_NODE 0x1
#define FDT_END_NODE   0x2
#define FDT_PROP       0x3
#define FDT_NOP        0x4
#define FDT_END        0x9

typedef struct {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;

	/* version 2 fields below */
	uint32_t boot_cpuid_phys;

	/* version 3 fields below */
	uint32_t size_dt_strings;

	/* version 17 fields below */
	uint32_t size_dt_struct;
} fdt_header;

typedef struct fdt_scan_node {
    struct fdt_scan_node *parent;
    const char *name;
    uint32_t address_cells;
    uint32_t size_cells;
} fdt_scan_node;

typedef struct {
    fdt_scan_node *node;
    const char *name;
    uint32_t *value;
    uint32_t len;
} fdt_scan_prop;

typedef struct {
    void (*open)(fdt_scan_node *node, void *buffer);
    void (*prop)(fdt_scan_prop *prop, void *buffer);
    void (*done)(fdt_scan_node *node, void *buffer);
    void (*close)(fdt_scan_node *node, void *buffer);
    void *buffer;
} fdt_proc;

uint32_t byte_reverse(uint32_t x);

#endif
