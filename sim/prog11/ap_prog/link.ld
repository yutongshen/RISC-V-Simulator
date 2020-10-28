OUTPUT_ARCH( "riscv" )

_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x1000;
_TEST_SIZE = DEFINED(_TEST_SIZE) ? _TEST_SIZE : 0x1000;

/*****************************************************************************
 * Define memory layout
 ****************************************************************************/
/* Specify the default entry point to the program */

ENTRY(_start)

/*****************************************************************************
 * Define the sections, and where they are mapped in memory 
 ****************************************************************************/

SECTIONS
{
  . = ALIGN(0x1000);
  .comment : { *(.comment) }
  .riscv.attributes : { *(.riscv.attributes) }
  . = 0x80000000;
  .text.init : { *(.text.init) }
  . = ALIGN(0x1000);
  .text : { *(.text) }
  . = ALIGN(0x1000);
  .data : { *(.data) }
  .bss : { *(.bss) }
  . = ALIGN(0x1000);
  _end = .;
}

