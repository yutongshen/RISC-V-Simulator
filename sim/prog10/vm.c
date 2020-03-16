#include <stdint.h>
#include "riscv_def.h"
#include "util.h"

#define N_PTE 1 << 9
#define N_PT 4
#define l1_pt pt[0]
#define l1_user_pt pt[1]
#define l2_user_pt pt[2]
#define l1_kernel_pt pt[3]

typedef uint64_t pte_t;

pte_t pt[N_PT][N_PTE] __attribute__((aligned(PGSIZE)));

void vm_boot()
{
    extern int _entry[];
    l1_pt[0] = ((pte_t) l1_user_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_user_pt[0] = ((pte_t) _entry >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_pt[N_PTE - 1] =
        ((pte_t) l1_kernel_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_kernel_pt[N_PTE - 1] = ((pte_t) _entry >> PAGE_SHIFT << PTE_PPN_SHIFT) |
                              PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V;
    // set CSR satp
    uint64_t satp_val = set_field((pte_t) &l1_pt, SATP_MODE, SATP_MODE_SV39);
    // printf("SATP: %08x\n", satp_val);
    write_csr(satp, satp_val);
    // uint64_t test;
    // read_csr(satp, test);
    // printf("SATP: %08x\n", test);
}
