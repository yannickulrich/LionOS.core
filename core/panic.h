/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  Panic
*/
#ifndef PANIC_H
#define PANIC_H

#define DE 0x00
#define DB 0x01
#define NMI 0x02
#define BP 0x03
#define OF 0x04
#define BR 0x05
#define UD 0x06
#define NM 0x07
#define DF 0x08
#define CSO 0x09
#define TS 0x0a
#define NP 0x0b
#define SS 0x0c
#define GP 0x0d
#define PF 0x0e
#define RE 0x0f
#define MF 0x10
#define AC 0x11
#define MC 0x12
#define XF 0x13
#define WE 0x15 //Wrong elf
#define NMF 0x16 //No mods
//#include "stdlib.c"
cpu_state* forceTaskSwitch(cpu_state *cpu);
void panic(int reason);
struct regs* fault_handler(struct regs* r);

#include "panic.c"
#endif

/*
Vektor 	 Bezeichnung
0x00 	#DE - Divide by Zero
0x01 	#DB - Debug
0x02 	NMI
0x03 	#BP - Break Point
0x04 	#OF - Over Flow
0x05 	#BR - Bound Range
0x06 	#UD - Invalid Opcode
0x07 	#NM - Device Not Available
0x08 	#DF - Double Fault
0x09 	Coprocessor Segment Overrun
0x0a 	#TS - Invalid TSS
0x0b 	#NP - Segment not Present
0x0c 	#SS - Stack
0x0d 	#GP - General Protection
0x0e 	#PF - Page Fault
0x0f 	Reserviert
0x10 	#MF - x87 Floating Point
0x11 	#AC - Alignment Check
0x12 	#MC - Machine Check
0x13 	#XF - SIMD Floating Point
0x14-0x1f 	Reserviert
*/
