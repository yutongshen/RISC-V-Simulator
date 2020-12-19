#include <string.h>
#include "fdt.h"
#include "riscv_def.h"
#include "mtrap.h"

extern void sysexit(uint16_t code);
extern void printm(const char *fmt, ...);
extern void __wfi(void);
extern void query_uart(void *dtb);
extern void query_finisher(void *dtb);
extern void query_hart(void *dtb);
extern void query_clint(void *dtb);
extern void query_plic(void *dtb);
extern volatile uintptr_t *mtime;

void basic_init(void)
{
    // Enable float
    write_csr(mstatus, MSTATUS_FS);

    // Enable MSIP
    write_csr(mie, MIP_MSIP)

    // Delegate interript
    write_csr(mideleg, MIP_SSIP | MIP_STIP | MIP_SEIP);

    // Delegate exception
    write_csr(medeleg, (1 << CAUSE_MISALIGNED_FETCH) |
                       (1 << CAUSE_USER_ECALL) |
                       (1 << CAUSE_BREAKPOINT) |
                       (1 << CAUSE_INSTRUCTION_PAGE_FAULT) |
                       (1 << CAUSE_LOAD_PAGE_FAULT) |
                       (1 << CAUSE_STORE_PAGE_FAULT));

    // Set supervisor legal address range for longest virtual - sv57
    uintptr_t pmpaddr = (1UL << (57 - 3)) - 1UL;
    uintptr_t pmpcfg  = PMP_R | PMP_W | PMP_X | set_field(0, PMP_A, PMP_NAPOT);
    asm volatile (
        "  la t0, 1f\n"
        "  csrrw t0, mtvec, t0\n"
        "  csrw pmpaddr0, %0\n"
        "  csrw pmpcfg0, %1\n"
        "  .align 2\n"
        "1:\n"
        "  csrw mtvec, t0\n"
        : : "r"(pmpaddr), "r"(pmpcfg) : "t0");
}

int init_primary(int mhartid, void *dtb)
{
    /* TM_PRINT="CPU%d: Initialize primary", mhartid */
    basic_init();

    query_uart(dtb);
    printm("Load UART sucessfully\n");

    query_finisher(dtb);
    query_hart(dtb);
    query_clint(dtb);
    query_plic(dtb);

    // test
    // /* TM_PRINT="TEST" */
    // hls_t *hls = HLS(1);
    // *(uint32_t *)((void *) hls->mtimecmp + 4) = 0x0123456789ABCDEF;
    // /* TM_PRINT="%lx", *HLS(1)->mtimecmp */
    // /* TM_PRINT="%lx", *HLS(2)->mtimecmp */

    // for (int i = 0; i < 100; ++i)
    // {
    //     /* TM_PRINT="mtime = %x", mtime[0] */
    //     /* TM_PRINT="mtime = %x", ((char *) mtime)[1] */
    // }

    return 0;
}

int init_secondary(int mhartid, void *dtb)
{
    /* TM_PRINT="CPU%d: Initialize secondary", mhartid */
    basic_init();
    __wfi();
    /* TM_PRINT="CPU%d: Initialize secondary done", mhartid */
    return 0;
}
