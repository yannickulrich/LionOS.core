/*  Bei _start soll die Ausfuehrung losgehen */
ENTRY(_start)

OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0x400000;
    .text : { *(.text*)             }
    .data : { *(.data*) *(.rodata*) }
    .bss :  { *(.bss*)              }
}


/*
 SCID   |      call        |    USED
--------+------------------+-------------
   0    | printChar        |      X
   1    | PN               |      X
   2    | clearScreen      |      X
   3    | changeColor      |      X
   4    | scanString       |      X
   5    | scanInt          |      X
   6    | panic            | 
   7    | initTask         |      X
   8    | idle             |      X
   9    | free             |      X
  10    | malloc           |      X
  11    | installCut       |      X
  12    | getCutHandler    | 
  13    | sleepTicks       |      X
  14    | getTime          |      X
  15    | addTimer         |      X
  16    | deleteTimer      |      X
        |                  | 
*/
