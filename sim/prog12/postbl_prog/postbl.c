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
extern void query_chosen(void *dtb);
extern uintptr_t get_fdt_all_size(fdt_header *header);
extern volatile uintptr_t *mtime;
extern volatile uint32_t *plic_priorities;
extern volatile const uint32_t *plic_pending;
extern uint32_t plic_ndevs;
extern void *kernel_start;
extern void *kernel_end;

void enter_supervisor_mode(void *addr, uintptr_t arg0, uintptr_t arg1)
{
  uintptr_t mstatus;
  read_csr(mstatus, mstatus);
  mstatus = set_field(mstatus, MSTATUS_MPP, PRV_S);
  mstatus = set_field(mstatus, MSTATUS_MPIE, 0);
  write_csr(mstatus, mstatus);
  register uintptr_t sp asm ("sp");
  write_csr(mscratch, sp);
  write_csr(mepc, addr);

  register uintptr_t a0 asm ("a0") = arg0;
  register uintptr_t a1 asm ("a1") = arg1;
  asm volatile ("mret" : : "r" (a0), "r" (a1));
}


void bootload(uintptr_t mhartid, void *dtb)
{
    void *output_dts = kernel_end; // TBD
    memcpy(output_dts, dtb, get_fdt_all_size(dtb));
    enter_supervisor_mode(kernel_start, mhartid, (uintptr_t) output_dts);
}

void basic_init(void)
{
    // Enable float
    write_csr(mstatus, MSTATUS_FS);

    // Enable MSIE
    write_csr(mie, MIP_MSIP);

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
    uintptr_t pmpaddr = -1UL;// (1UL << (57 - 3)) - 1UL;
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

void plic_irq_init()
{
    for (uint32_t i = 1; i < plic_ndevs; ++i)
    {
        plic_priorities[i] = 1;
    }
}

void plic_hart_init()
{
    hls_t *hls = HLS_SELF;
    uint32_t n = (plic_ndevs >> 5) + !!(plic_ndevs & 0x1f);
    for (uint32_t i = 0; i < n; ++i)
    {
        if (hls->plic_sie) hls->plic_sie[i] = -1U;
    }

    hls->plic_mth->thresh = 1;
    if (hls->plic_sth) hls->plic_sth->thresh = 0;
}

void irq_test()
{
    hls_t *hls = HLS_SELF;
    // MTIP
    *hls->mtimecmp = 0L;
    assert(read_csr_directly(mip) & MIP_MTIP);
    *hls->mtimecmp = -1L;
    assert(!(read_csr_directly(mip) & MIP_MTIP));
    // MSIP
    *hls->ipi = 1;
    assert(read_csr_directly(mip) & MIP_MSIP);
    *hls->ipi = 0;
    assert(!(read_csr_directly(mip) & MIP_MSIP));
}

int init_primary(uintptr_t mhartid, void *dtb)
{
    /* TM_PRINT="CPU%d: Initialize primary", mhartid */
    basic_init();

    query_uart(dtb);
    printm("Load UART sucessfully\n");

    query_finisher(dtb);
    query_hart(dtb);
    query_clint(dtb);
    irq_test();
    query_plic(dtb);
    plic_irq_init();
    plic_hart_init();
    query_chosen(dtb);

    bootload(mhartid, dtb);

    // test
    // /* TM_PRINT="TEST" */
    // set_csr(mie, MIP_MEIP);
    // set_csr(mie, MIP_SEIP);
    // // *hls->plic_sie = 0xffffffff;
    // *TMDL_RG_TM_FORCE_IRQ_32P |= 0xff;
    // __wfi();
    // hls_t *hls = HLS(3);
    // *hls->ipi = 1;
    // *hls->ipi = 0;

    // for (int i = 0; i < 10; ++i)
    // {
    //     /* TM_PRINT="IRQ_PENDING: %x", *plic_pending */
    // }

    return 0;
}

int init_secondary(uintptr_t mhartid, void *dtb)
{
    /* TM_PRINT="CPU%d: Initialize secondary", mhartid */
    basic_init();
    irq_test();



    // plic_hart_init();
    /* TM_PRINT="CPU%d: Initialize secondary done", mhartid */
    hls_t *hls = HLS_SELF;
    /* TM_PRINT="CPU%d: IRQ_%d triggered", mhartid, hls->plic_sth->id */
    __wfi();
    return 0;
}
