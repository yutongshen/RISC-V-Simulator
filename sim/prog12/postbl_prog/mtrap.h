#ifndef __MTRAP__
#define __MTRAP__
#include <stdint.h>

typedef struct {
  volatile uintptr_t* mtimecmp;
  volatile uint32_t* ipi;
  volatile uint32_t ipi_pending;
  volatile uint32_t* plic_mth;
  volatile uint32_t* plic_mie;
  volatile uint32_t* plic_sth;
  volatile uint32_t* plic_sie;
} hls_t;


#define read_csr_directly(name) ({ \
    uintptr_t __csr; \
    asm volatile ( \
        "csrr %0, " #name \
        : "=&r"(__csr) ); \
    __csr; })
#define __HLS ({ register uintptr_t sp asm ("sp"); (sp + PGSIZE & -PGSIZE); })
#define HLS_SELF ((hls_t *) __HLS)
#define HLS(x) ((hls_t *) (__HLS + ((x - read_csr_directly(mhartid)) << PGSHFT)))
#define die(str, ...) ({ printm("%s:%d: " str "\n", __FILE__, __LINE__, ##__VA_ARGS__); sysexit(1); })
#define assert(x) ({ if (!(x)) { die("assertion fail: %s", #x ); } })

#endif
