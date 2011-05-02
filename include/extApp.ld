/*  Bei _start soll die Ausfuehrung losgehen */
ENTRY(main)

OUTPUT_FORMAT(elf32-i386)
/*  Bei _start soll die Ausfuehrung losgehen */


/*
 * Hier wird festgelegt, in welcher Reihenfolge welche Sektionen in die Binary
 * geschrieben werden sollen
 */
SECTIONS
{
    /* Das Programm wird an 2 MB geladen */
    . = 0x600000;

    .text : {
        *(.text)
    }
    .data ALIGN(4096) : {
        *(.data)
    }
    .rodata ALIGN(4096) : {
        *(.rodata)
    }
    .bss ALIGN(4096) : {
        *(.bss)
    }
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
