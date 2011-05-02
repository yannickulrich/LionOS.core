/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   Init
 * Module: 0x05 - INIT
 */

#include "datatypes.h"
#include "io.h"
#include "elf.h"
//#include "misc.h"
#include "panic.h"
#include "driver/floppy.h"
#include "fat.h"
#include "mm/pmm.h"
#include "tasking.h"
#include "core.h"
#include "syscall.h"
#include "driver/cmos.h"
#include "time.h"
#include "mm/vmm.h"

void defaultTask()
{
	while(1);
}
void init(struct multiboot_info *mb_info)
{	
	int line;
	asm("cli");
	initScreen();
	printLogo();
	setFix(10);
	changeColor(0x0, 0xF);
	if (!DEBUG)
	{
		for(line = 0; line < 33; line++) kprintf(" ");
		kprintf("%c Loading... %c", (char) 174, (char) 175);
		setCursorVisibility(false);
	}
	else
	{
		kprintf("  %c KernelDBG Mode \n", (char) 175);
	}
	pc("[05.%s] +makeKbReady", dl);
	outportb(0x3d8, 0xff);
	setAskKeycodes(false);
	initKeycodes();
	//clearscreen();
	pmminit(mb_info); 
	vmmInit();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	installKeyboard();
	installTimer();
	startTasking();
	initFDD();
	initFAT();
	asm("cli");
	changeColor(0x0, 0xF);
	kprintf(" \b");
	//Here we must load apps...
	DEBUG_MSG("MM:       Init Dummy Task...");
	PS("[05.%s] +initDummyTask", en(__LINE__));
	initTask(defaultTask, 0, NIL, "");
	dbg(true);
	//startTasking();
	PS("[05.%s] +multibootinfo.mods == 0", dl);
		DEBUG_MSG("INIT:     Run App...");
	//if (DEBUG) dbg(1);
	
	//if (DEBUG) while(1); 
	if (mb_info->mb_mods_count == 0) {
		PS("[05.000076] +echo \"Error: No module given.\"");
		PS("[05.000076] +echo \"The system halts now.\"");
		dbg(false);
		if (!DEBUG) panic(NMF);
		DEBUG_MSG("            Cause: No module given.\n");
		DEBUG_MSG("INIT:     The system halts immediatly");
		PS("[05.%s] +halt", dl);
		asm("cli");
		asm("hlt");
	}
	else 
	{
		dbg(true);
		PS("[05.%s] +clear screen", en(__LINE__));
		PS("[05.%s] +unlock screen", dl);
		PS("[05.%s] ~WARNING: Further notes will be suppressed.", dl);
		supressPostMessages = true;
		setCursorVisibility(true);
		setFix(0);
		changeColor(0x0, 0xF);
		clearscreen();
		struct multiboot_module* modules = mb_info->mb_mods_addr;
		//pc("[05.000087] +init grub app");

		/*pageDir_t page = vmmCreateContext();
		void *paddr = pmm_malloc(PAGE_SIZE);
		
		kmemset((void*) page, 0, PAGE_SIZE);
	
		vmmMapPage( page, (void*) 0xfffff000, paddr, USER_PRV ); //0xfffff000
		vmmActivateContext( page );*/

		initELF((void*) modules[0].start, 100, modules->end - modules->start, modules->cmdline);
		asm("sti"); 
	}
	//clearscreen();
	while(1);//{asm("sti");}
}
