#include <stdint.h>
#include "mmap_soc.h"
#include "riscv_def.h"

#define MEGAPAGE (pte_t)(PGSIZE << 9)

#define N_PTE (1 << 9)
#define N_PT 4
#define l1_pt pt[0]
#define l1_user_pt pt[1]
#define l2_user_pt pt[2]
#define l1_kernel_pt pt[3]

#define pa2kva(x) ((pte_t)(x) - (pte_t) _entry - MEGAPAGE)
#define pa2uva(x) ((pte_t)(x) - (pte_t) _entry)

typedef uint64_t pte_t;

pte_t pt[N_PT][N_PTE] __attribute__((aligned(PGSIZE)));

void vm_boot()
{
    uint64_t _entry = DDR_1_BASE;

    // build user and kernel page
    l1_pt[0] = ((pte_t) l1_user_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_user_pt[0] = ((pte_t) l2_user_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_pt[N_PTE - 1] =
        ((pte_t) l1_kernel_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_kernel_pt[N_PTE - 1] = ((pte_t) _entry >> PAGE_SHIFT << PTE_PPN_SHIFT) |
                              PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V;

    // set CSR satp
    uint64_t satp_val =
        set_field((pte_t) &l1_pt >> PAGE_SHIFT, SATP_MODE, SATP_MODE_SV39);
    write_csr(satp, satp_val);

    // set supervisor legal address range for longest virtual - sv57
    write_csr(pmpaddr0, (1UL << (57 - 3)) - 1U);
    write_csr(pmpcfg0, PMP_R | PMP_W | PMP_X | set_field(0, PMP_A, PMP_NAPOT));

    // delegate interrupt for supervisor
    write_csr(mideleg, MIP_SSIP);

    // // Enable supervisor interrupt
    // set_csr(mstatus, MSTATUS_SIE);
    // set_csr(sie, MIP_SSIP);

    // delegate exception for supervisor
    write_csr(medeleg, (1 << CAUSE_MISALIGNED_FETCH) | (1 << CAUSE_USER_ECALL) |
                           (1 << CAUSE_BREAKPOINT) |
                           (1 << CAUSE_INSTRUCTION_PAGE_FAULT) |
                           (1 << CAUSE_LOAD_PAGE_FAULT) |
                           (1 << CAUSE_STORE_PAGE_FAULT));

    // // set supervisor trap entry
    // write_csr(stvec, pa2kva(&strap_entry));

    // // set supervisor trapframe pointer
    // uint64_t scratch;
    // read_csr(mscratch, scratch);
    // write_csr(sscratch, pa2kva(scratch - TRAPFRAM_SIZE));

    // switch to s-mode
    write_csr(mepc, pa2kva(DDR_1_BASE));
    uint64_t mstatus_val;
    read_csr(mstatus, mstatus_val);
    mstatus_val &= ~MSTATUS_MPP;
    mstatus_val |= PRV_S << 11;
    write_csr(mstatus, mstatus_val);
    asm volatile("mret");
}
