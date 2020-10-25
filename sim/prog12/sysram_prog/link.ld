OUTPUT_ARCH( "riscv" )

/*****************************************************************************
 * Define memory layout
 ****************************************************************************/
STACK_POINTER = 0x00030000;

/* Specify the default entry point to the program */

ENTRY(_start)

/*****************************************************************************
 * Define the sections, and where they are mapped in memory 
 ****************************************************************************/

SECTIONS
{
  . = 0x00010000;
  .text : { setup.o(.text) }
  .text : { *(.text) }
  _end = .;
}

