#include <stdint.h>
#include "mmap_soc.h"
#include "riscv_def.h"
#include "type_def.h"
#include "util.h"

#define N_PAGE 32  // 0x10000 ~ 0x2ffff
#define FREE_PAGE_BASE 0x10000
#define MEGAPAGE (pte_t)(PGSIZE << 9)

#define N_PTE (1 << 9)
#define N_PT 4
#define l1_pt(n) pt[n][0]
#define l1_user_pt(n) pt[n][1]
#define l2_user_pt(n) pt[n][2]
#define l1_kernel_pt(n) pt[n][3]

#define pa2kva(x) ((pte_t)(x) - (pte_t) _start - MEGAPAGE)
#define pa2uva(x) ((pte_t)(x) - (pte_t) _start)
#define uva2kva(x) ((pte_t)(x) -MEGAPAGE)

typedef uint64_t pte_t;
typedef struct {
    uint32_t pid;
    pte_t addr;
    void *next;
} list_t;

static uint32_t pid;
list_t free_page_node[N_PAGE];
list_t *free_page_node_head, *free_page_node_tail;
list_t *page_node_head, *node_tail;

#define N_PAGE_TBL 10

pte_t pt[N_PAGE_TBL][N_PT][N_PTE] __attribute__((aligned(PGSIZE)));
list_t free_page_tbl_node[N_PAGE];
list_t *free_page_tbl_head, *free_page_tbl_tail;
list_t *page_tbl_head, *page_tbl_tail;


extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;
extern void pop_trapframe(trapframe_t *tf);
extern void trap_entry();
extern int user_space();
extern void _start();


void list_add_node(list_t *head, list_t *node) {
    node->next = head;
    head = node;
}

void fault_handle(uint64_t addr, uint64_t cause)
{
    addr = addr >> PAGE_SHIFT << PAGE_SHIFT;
    pte_t *pte = l2_user_pt(pid) + ((addr >> PAGE_SHIFT) & ((1UL << 9) - 1UL));
    if (*pte & PTE_V) {
        assert(*pte & PTE_A);
        if (!(*pte & PTE_D)) {
            assert(cause == CAUSE_STORE_PAGE_FAULT);
            *pte |= PTE_D;
        }
        return;
    }

    // allocat new page
    assert(free_page_node_head);
    pte_t paddr = free_page_node_head->addr;
    free_page_node_head = free_page_node_head->next;
    *pte = paddr >> PAGE_SHIFT << PTE_PPN_SHIFT | PTE_A | PTE_U | PTE_R |
           PTE_W | PTE_X | PTE_V;
    if (cause == CAUSE_STORE_PAGE_FAULT)
        *pte |= PTE_D;

    set_csr(sstatus, MSTATUS_SUM);
    memcpy((uint64_t *) addr, (uint64_t *) uva2kva(addr), PGSIZE);
    clear_csr(sstatus, MSTATUS_SUM);
}

void page_tbl_allocat() {
    assert(free_page_tbl_head);

    uint32_t pid = free_page_tbl_head->pid;

    // build user and kernel page
    l1_pt(pid)[0] = ((pte_t) l1_user_pt(pid) >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_user_pt(pid)[0] = ((pte_t) l2_user_pt(pid) >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_pt(pid)[N_PTE - 1] =
        ((pte_t) l1_kernel_pt(pid) >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_kernel_pt(pid)[N_PTE - 2] =
        ((pte_t) BRIDGE_0_BASE >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_D | PTE_A |
        PTE_R | PTE_W | PTE_X | PTE_V;
    l1_kernel_pt(pid)[N_PTE - 1] = ((pte_t) _start >> PAGE_SHIFT << PTE_PPN_SHIFT) |
                              PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V;

    // set CSR satp
    uint64_t satp_val =
        set_field((pte_t) &l1_pt(pid) >> PAGE_SHIFT, SATP_MODE, SATP_MODE_SV39);
    write_csr(satp, satp_val);

    free_page_tbl_head = free_page_tbl_head->next;
}

void vm_boot()
{
    // initialize free page table node
    for (uint32_t i = 0; i < N_PAGE_TBL; ++i) {
        free_page_tbl_node[i].pid  = i;
        free_page_tbl_node[i].addr = (pte_t) (pt + i);
        free_page_tbl_node[i].next = (void *) pa2kva(free_page_tbl_node + i + 1);
    }
    free_page_tbl_node[N_PAGE_TBL - 1].next = 0;
    free_page_tbl_head =  (list_t *) pa2kva(free_page_tbl_node);
    free_page_tbl_tail =  (list_t *) pa2kva(free_page_tbl_node + N_PAGE_TBL - 1);

    page_tbl_head = 0;
    page_tbl_tail = 0;


    // initialize free page node
    for (uint32_t i = 0; i < N_PAGE; ++i) {
        free_page_node[i].pid  = -1;
        free_page_node[i].addr = FREE_PAGE_BASE + i * 0x1000;
        free_page_node[i].next = (void *) pa2kva(free_page_node + i + 1);
    }
    free_page_node[N_PAGE - 1].next = 0;
    free_page_node_head = (list_t *) pa2kva(free_page_node);
    free_page_node_tail = (list_t *) pa2kva(free_page_node + N_PAGE - 1);

    page_node_head = 0;
    page_node_tail = 0;

    // set supervisor legal address range for longest virtual - sv57
    write_csr(pmpaddr0, (1UL << (57 - 3)) - 1U);
    write_csr(pmpcfg0, PMP_R | PMP_W | PMP_X | set_field(0, PMP_A, PMP_NAPOT));

    // delegate exception for supervisor
    write_csr(medeleg, (1 << CAUSE_MISALIGNED_FETCH) | (1 << CAUSE_USER_ECALL) |
                           (1 << CAUSE_BREAKPOINT) |
                           (1 << CAUSE_INSTRUCTION_PAGE_FAULT) |
                           (1 << CAUSE_LOAD_PAGE_FAULT) |
                           (1 << CAUSE_STORE_PAGE_FAULT));

    // set supervisor trap entry
    write_csr(stvec, pa2kva(&trap_entry));

    // set supervisor trapframe pointer
    uint64_t scratch;
    read_csr(mscratch, scratch);
    write_csr(sscratch, pa2kva(scratch));

    // switch to user mode
    pid = 0;
    trapframe_t tf;
    memset(&tf, 0, sizeof(tf));
    tf.epc = pa2uva(&user_space);
    pop_trapframe(&tf);
}
