#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "mmap_soc.h"
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

#define pa2kva(x) ((pte_t)(x) - (pte_t) _start - MEGAPAGE)
#define pa2uva(x) ((pte_t)(x) - (pte_t) _start)
#define uva2kva(x) ((pte_t)(x) - MEGAPAGE)

#define PERI_PA2KVA(ADDR, TYPE) ((volatile TYPE *)(ADDR - 2 * N_PTE * MEGAPAGE))

typedef uint64_t pte_t;
typedef struct {
    pte_t addr;
    void *next;
} page_list_t;

extern void pop_trapframe(trapframe_t *tf);
extern void trap_entry();
extern int user_space();
extern void _start();

pte_t pt[N_PT][N_PTE] __attribute__((aligned(PGSIZE)));

page_list_t free_node[N_PAGE];
page_list_t *free_node_head, *free_node_tail;

void *copy_to_user(void *_user, void *_kern, uint32_t _len) {
    set_csr(sstatus, MSTATUS_SUM);
    memcpy((uint64_t *) _user, (uint64_t *) _kern, _len);
    clear_csr(sstatus, MSTATUS_SUM);
    return _user;
}

void *copy_from_user(void *_kern, void *_user, uint32_t _len) {
    set_csr(sstatus, MSTATUS_SUM);
    memcpy((uint64_t *) _kern, (uint64_t *) _user, _len);
    clear_csr(sstatus, MSTATUS_SUM);
    return _kern;
}

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
    // build user and kernel page
    l1_pt[0] = ((pte_t) l1_user_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_user_pt[0] = ((pte_t) l2_user_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_pt[N_PTE - 2] =
        ((pte_t) 0 >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_D | PTE_A |
        PTE_R | PTE_W | PTE_X | PTE_V;
    l1_pt[N_PTE - 1] =
        ((pte_t) l1_kernel_pt >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_V;
    l1_kernel_pt[N_PTE - 2] =
        ((pte_t) BRIDGE_0_BASE >> PAGE_SHIFT << PTE_PPN_SHIFT) | PTE_D | PTE_A |
        PTE_R | PTE_W | PTE_X | PTE_V;
    l1_kernel_pt[N_PTE - 1] = ((pte_t) _start >> PAGE_SHIFT << PTE_PPN_SHIFT) |
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
    trapframe_t tf;
    memset(&tf, 0, sizeof(tf));
    tf.epc = pa2uva(&user_space);
    pop_trapframe(&tf);
}

void write_tohost(uint64_t val)
{
    __sync_synchronize();

    *PERI_PA2KVA(HTIF_RG_TOHOST, uint64_t) = val;
    while (!*PERI_PA2KVA(HTIF_RG_FROMHOST, uint64_t));
    *PERI_PA2KVA(HTIF_RG_FROMHOST, uint64_t) = 0;

    __sync_synchronize();
}

trapframe_t *ecall_handler(trapframe_t *ft)
{
    if (ft->a0) return write_tohost(ft->a1), ft;
    else return ((void *(*)(trapframe_t *))ft->a1 - 0x200000)(ft);
}

int putchar_kern(int ch)
{
    write_tohost(SYSCALL_PUTCHAR << 48 | ch);
    return 0;
}

int _puts_kern(const char *s)
{
    int res = 0;
    while (*s) {
        putchar_kern(*(s++));
        ++res;
    }

    return res;
}

int printk(const char *format, ...)
{
    const char *ptr = format;
    int res = 0;

    va_list va;
    va_start(va, format);

    while (*ptr) {
        if (*ptr == '%') {
            int min_len = 0;
            char fill_char = 0;
        loop1:
            switch (*(++ptr)) {
            case 'c':
                assert(!(min_len || fill_char));
                ++res;
                putchar_kern(va_arg(va, int));
                break;
            case 's':
                assert(!(min_len || fill_char));
                res += _puts_kern(va_arg(va, const char *));
                break;
            case 'p': {
                assert(!(!fill_char ^ !min_len));
                uint64_t n = va_arg(va, uint64_t);
                res += _puts_kern("0x");
                res += _puts_kern(itoa(n, 16, min_len, min_len ? fill_char : '0'));
            } break;
            case 'x': {
                assert(!(!fill_char ^ !min_len));
                uint64_t n = va_arg(va, uint64_t);
                res += _puts_kern(itoa(n, 16, min_len, min_len ? fill_char : '0'));
            } break;
            case 'd': {
                assert(!(!fill_char ^ !min_len));
                int64_t n = va_arg(va, int64_t);
                if (n < 0) {
                    ++res;
                    putchar_kern('-');
                    n = -n;
                }
                res += _puts_kern(itoa(n, 10, min_len, min_len ? fill_char : '0'));
            } break;
            case 'f': {
                assert(!(!fill_char ^ !min_len));
                double n = va_arg(va, double);
                res += _puts_kern(ftoa(n));
            } break;
            case '%':
                ++res;
                putchar_kern('%');
                break;
            case '1':
                assert(fill_char);
                min_len *= 10;
                min_len += 1;
                goto loop1;
            case '2':
                assert(fill_char);
                min_len *= 10;
                min_len += 2;
                goto loop1;
            case '3':
                assert(fill_char);
                min_len *= 10;
                min_len += 3;
                goto loop1;
            case '4':
                assert(fill_char);
                min_len *= 10;
                min_len += 4;
                goto loop1;
            case '5':
                assert(fill_char);
                min_len *= 10;
                min_len += 5;
                goto loop1;
            case '6':
                assert(fill_char);
                min_len *= 10;
                min_len += 6;
                goto loop1;
            case '7':
                assert(fill_char);
                min_len *= 10;
                min_len += 7;
                goto loop1;
            case '8':
                assert(fill_char);
                min_len *= 10;
                min_len += 8;
                goto loop1;
            case '9':
                assert(fill_char);
                min_len *= 10;
                min_len += 9;
                goto loop1;
            default:
                assert(!min_len);
                fill_char = *ptr;
                goto loop1;
            }
        } else {
            ++res;
            putchar_kern(*ptr);
        }
        ++ptr;
    }
    return res;
}
