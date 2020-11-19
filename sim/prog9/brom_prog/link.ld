OUTPUT_ARCH( "riscv" )

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
  . = 0x00000000;
  .text : { setup.o(.text) }
  .text : { *(.text) }
  .comment : { *(.comment) }
  .riscv.attributes : { *(.riscv.attributes) }
  . = 0x00030000;
  STACK_POINTER = .;
}

