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
  . = 0xffffffffffe00000;
  .text.init : { *(.text.init) }
  . = 0xffffffffffe02000;
  .text : { *(.text) }
  . = ALIGN(0x1000);
  .data : { *(.data) }
  .bss : { *(.bss) }
  . = ALIGN(0x1000);
  _end = .;
  STACK_POINTER = .;
}

