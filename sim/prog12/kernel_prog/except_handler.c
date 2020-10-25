#include <stdint.h>
#include "riscv_def.h"
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
#define uva2kva(x) ((pte_t)(x) -MEGAPAGE)
typedef uint64_t pte_t;
typedef struct {
    pte_t addr;
    void *next;
} page_list_t;

pte_t **pt;

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

void set_pagetbl()
{
    uint64_t satp;
    read_csr(satp, satp);
    pt = (pte_t **) (satp << PAGE_SHIFT);

    // initialize free page node
    for (uint32_t i = 0; i < N_PAGE; ++i) {
        free_node[i].addr = FREE_PAGE_BASE + i * 0x1000;
        free_node[i].next = free_node + i + 1;
    }
    free_node[N_PAGE - 1].next = 0;
    free_node_head = free_node;
    free_node_tail = free_node + N_PAGE - 1;
}
