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
  . = 0x80000000;
  .text : { setup.o(.text) }
  .text : { *(.text) }
}

