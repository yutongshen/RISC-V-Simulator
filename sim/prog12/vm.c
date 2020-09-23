#include <stdint.h>
#include "riscv_def.h"
#include "type_def.h"
#include "util.h"

#define N_PAGE 32  // 0x10000 ~ 0x2ffff
#define FREE_PAGE_BASE 0x10000
#define MEGAPAGE (pte_t)(PGSIZE << 9)

#define N_PTE (1 << 9)
#define N_PT 4
#define l1_pt pt[0]
#define l1_user_pt pt[1]
#define l2_user_pt pt[2]
#define l1_kernel_pt pt[3]

#define pa2kva(x) ((pte_t)(x) - (pte_t) _entry - MEGAPAGE)
#define pa2uva(x) ((pte_t)(x) - (pte_t) _entry)
#define uva2kva(x) ((pte_t)(x) -MEGAPAGE)

typedef uint64_t pte_t;
typedef struct {
    pte_t addr;
    void *next;
} page_list_t;

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;
extern void pop_trapframe_m(trapframe_t *tf);
extern void trap_entry_s();
extern int user_space();

pte_t pt[N_PT][N_PTE] __attribute__((aligned(PGSIZE)));

page_list_t free_node[N_PAGE];
page_list_t *free_node_head, *free_node_tail;

void fault_handle(uint64_t addr, uint64_t cause)
{
    addr = addr >> PAGE_SHIFT << PAGE_SHIFT;
    pte_t *pte = l2_user_pt + ((addr >> PAGE_SHIFT) & ((1UL << 9) - 1UL));
    if (*pte & PTE_V) {
        assert(*pte & PTE_A);
        if (!(*pte & PTE_D)) {
            assert(cause == CAUSE_STORE_PAGE_FAULT);
            *pte |= PTE_D;
        }
        return;
    }

    // allocat new page
    assert(free_node_head);
    pte_t paddr = free_node_head->addr;
    free_node_head = free_node_head->next;
    *pte = paddr >> PAGE_SHIFT << PTE_PPN_SHIFT | PTE_A | PTE_U | PTE_R |
           PTE_W | PTE_X | PTE_V;
    if (cause == CAUSE_STORE_PAGE_FAULT)
        *pte |= PTE_D;

    set_csr(sstatus, MSTATUS_SUM);
    memcpy((uint64_t *) addr, (uint64_t *) uva2kva(addr), PGSIZE);
    clear_csr(sstatus, MSTATUS_SUM);
}

void vm_boot()
{
    extern int _entry[];

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

    // initialize free page node
    for (uint32_t i = 0; i < N_PAGE; ++i) {
        free_node[i].addr = FREE_PAGE_BASE + i * 0x1000;
        free_node[i].next = (void *) pa2kva(free_node + i + 1);
    }
    free_node[N_PAGE - 1].next = 0;
    free_node_head = (page_list_t *) pa2kva(free_node);
    free_node_tail = (page_list_t *) pa2kva(free_node + N_PAGE - 1);

    // set supervisor legal address range for longest virtual - sv57
    write_csr(pmpaddr0, (1UL << (57 - 3)) - 1U);
    write_csr(pmpcfg0, PMP_R | PMP_W | PMP_X | set_field(0, PMP_A, PMP_NAPOT));

    // delegate interrupt for supervisor
    write_csr(mideleg, MIP_SSIP);

    // Enable supervisor interrupt
    set_csr(mstatus, MSTATUS_SIE);
    set_csr(sie, MIP_SSIP);

    // delegate exception for supervisor
    write_csr(
        medeleg,
        (1 << CAUSE_MISALIGNED_FETCH) | (1 << CAUSE_USER_ECALL) |
            (1 << CAUSE_BREAKPOINT) | (1 << CAUSE_INSTRUCTION_PAGE_FAULT) |
            (1 << CAUSE_LOAD_PAGE_FAULT) | (1 << CAUSE_STORE_PAGE_FAULT));

    // set supervisor trap entry
    write_csr(stvec, pa2kva(&trap_entry_s));

    // set supervisor trapframe pointer
    uint64_t scratch;
    read_csr(mscratch, scratch);
    write_csr(sscratch, pa2kva(scratch - TRAPFRAM_SIZE));

    // switch to user mode
    trapframe_t tf;
    memset(&tf, 0, sizeof(tf));
    tf.epc = pa2uva(&user_space);
    pop_trapframe_m(&tf);
}
