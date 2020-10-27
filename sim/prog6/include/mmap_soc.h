// Memory Mapped Defination

// ==============================
//  BROM_BASE
// ==============================
#define BROM_BASE 0x00000000

// ==============================
//  SRAM_0_BASE
// ==============================
#define SRAM_0_BASE 0x00010000

// ==============================
//  SRAM_1_BASE
// ==============================
#define SRAM_1_BASE 0x00020000

// ==============================
//  CLST_0_BASE
// ==============================
#define CLST_0_BASE 0x00040000
#define CLST_0_RG_PWR_REQ (CLST_0_BASE + 0x00)
#define CLST_0_RG_CPU0_PC (CLST_0_BASE + 0x10)
#define CLST_0_RG_CPU1_PC (CLST_0_BASE + 0x14)
#define CLST_0_RG_CPU2_PC (CLST_0_BASE + 0x18)
#define CLST_0_RG_CPU3_PC (CLST_0_BASE + 0x1c)
#define CLST_0_RG_CPU4_PC (CLST_0_BASE + 0x20)
#define CLST_0_RG_CPU5_PC (CLST_0_BASE + 0x24)
#define CLST_0_RG_CPU6_PC (CLST_0_BASE + 0x28)
#define CLST_0_RG_CPU7_PC (CLST_0_BASE + 0x2c)
#define CLST_0_RG_CPU0_MIP (CLST_0_BASE + 0x30)
#define CLST_0_RG_CPU1_MIP (CLST_0_BASE + 0x34)
#define CLST_0_RG_CPU2_MIP (CLST_0_BASE + 0x38)
#define CLST_0_RG_CPU3_MIP (CLST_0_BASE + 0x3c)
#define CLST_0_RG_CPU4_MIP (CLST_0_BASE + 0x40)
#define CLST_0_RG_CPU5_MIP (CLST_0_BASE + 0x44)
#define CLST_0_RG_CPU6_MIP (CLST_0_BASE + 0x48)
#define CLST_0_RG_CPU7_MIP (CLST_0_BASE + 0x4c)

// ==============================
//  BRIDGE_0_BASE
// ==============================
#define BRIDGE_0_BASE 0x10000000

// ==============================
//  FINISHER_BASE
// ==============================
#define FINISHER_BASE 0x10000000
#define FINISHER_RG_FINISH (FINISHER_BASE + 0x0)

// ==============================
//  UART_BASE
// ==============================
#define UART_BASE 0x10001000
#define UART_RG_TXFIFO (UART_BASE + 0x00)
#define UART_RG_RXFIFO (UART_BASE + 0x04)
#define UART_RG_TXCTRL (UART_BASE + 0x08)
#define UART_RG_RXCTRL (UART_BASE + 0x0C)
#define UART_RG_IE (UART_BASE + 0x10)
#define UART_RG_IP (UART_BASE + 0x14)
#define UART_RG_DIV (UART_BASE + 0x18)

// ==============================
//  CLINT_BASE
// ==============================
#define CLINT_BASE 0x10010000
#define CLINT_RG_MSIP (CLINT_BASE + 0x0000)
#define CLINT_RG_TIMECMP (CLINT_BASE + 0x4000)
#define CLINT_RG_TIME (CLINT_BASE + 0xbff8)

// ==============================
//  PLIC_BASE
// ==============================
#define PLIC_BASE 0x14000000
#define PLIC_RG_PRIOR (PLIC_BASE + 0x000000)
#define PLIC_RG_PEND (PLIC_BASE + 0x001000)
#define PLIC_RG_ENABLE (PLIC_BASE + 0x002000)
#define PLIC_RG_PRIOR_TH (PLIC_BASE + 0x200000)
#define PLIC_RG_INTID (PLIC_BASE + 0x200004)

// ==============================
//  DDR_0_BASE
// ==============================
#define DDR_0_BASE 0x20000000

// ==============================
//  DDR_1_BASE
// ==============================
#define DDR_1_BASE 0x80000000

// ==============================
//  FLASH_BASE
// ==============================
#define FLASH_BASE 0x100000000L

