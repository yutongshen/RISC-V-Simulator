OUTPUT_ARCH( "riscv" )

_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x1000;
_TEST_SIZE = DEFINED(_TEST_SIZE) ? _TEST_SIZE : 0x1000;

/*****************************************************************************
 * Define memory layout
 ****************************************************************************/
MEMORY {
  rom : ORIGIN = 0x00000000, LENGTH = 0x00002000
  imem : ORIGIN = 0x00010000, LENGTH = 0x00010000
  dmem : ORIGIN = 0x00020000, LENGTH = 0x00010000
  dram_i : ORIGIN = 0x20000000, LENGTH = 0x00100000
  dram_d : ORIGIN = 0x20100000, LENGTH = 0x00100000
}

/* Specify the default entry point to the program */

ENTRY(_start)

/*****************************************************************************
 * Define the sections, and where they are mapped in memory 
 ****************************************************************************/
SECTIONS {
  .text0 : {
    setup.o(.text);
    boot.o(.text);
  } > rom

  .text1 : {
    _imem_start = .;
    isr.o(.text);
    *(.text);
    *(.text.*);
  } > imem AT > dram_i

  .init : {
    KEEP (*(.init))
  } > imem AT > dram_i

  .fini : {
    KEEP (*(.fini))
  } > imem AT > dram_i

  .rodata : {
    __rodata_start = .;
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
    __rodata_end = .;
  } > imem AT > dram_i

  _dram_i_start = ORIGIN(dram_i);
  _dram_i_end = ORIGIN(dram_i) + . - ORIGIN(imem);

  _test : {
    . = ALIGN(4);
    _dmem_start = .;
    _test_start = .;
    . += _TEST_SIZE;
    _test_end = .;
  } > dmem

  .sbss : {
    __sbss_start = .;
    *(.sbss)
    *(.sbss.*)
    *(.gnu.linkonce.sb.*)
    __sbss_end = .;
  } > dmem

  .sdata : {
    __sdata_paddr_start = LOADADDR(.sdata);
    __sdata_start = .;
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    __sdata_end = .;
  } > dmem AT > dram_d

  .data : {
    . = ALIGN(4);
    __data_paddr_start = LOADADDR(.data);
    __data_start = .;
    *(.data)
    *(.data.*)
    *(.gnu.linkonce.d.*)
    __data_end = .;
  } > dmem AT > dram_d

  .bss : {
    . = ALIGN(4);
    __bss_start = .;
    *(.bss)
    *(.bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  } > dmem

  .stack : {
    . = ALIGN(4);
    _stack_end = .;
    . += _STACK_SIZE;
    _stack = .;
    __stack = _stack;
  } > dmem

  . = ORIGIN(dmem) + LENGTH(dmem) - 4;
  _sim_end = .;
  . += 4;
  _end = .;
}
