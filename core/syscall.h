/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  Syscall 
*/

#ifndef SYSCALL_H
#define SYSCALL_H
#define outb outportb
#define inb inportb
#include "io.h"

unsigned inportb(unsigned port);
void outportb(unsigned port, unsigned val);
void reboot();


/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  IDT
*/
#include "datatypes.h"
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
}__attribute__((packed)); //prevent compiler optimization

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
}__attribute__((packed)); //prevent compiler optimization
struct idt_entry idt[256];
struct idt_ptr   idt_register;
void idt_install();
	
/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  ISR
*/
extern void isr0();  
extern void isr1();  
extern void isr2();  
extern void isr3();
extern void isr4(); 
extern void isr5();  
extern void isr6();  
extern void isr7();
extern void isr8();  
extern void isr9();  
extern void isr10(); 
extern void isr11();
extern void isr12(); 
extern void isr13(); 
extern void isr14(); 
extern void isr15();
extern void isr16(); 
extern void isr17(); 
extern void isr18(); 
extern void isr19();
extern void isr20(); 
extern void isr21(); 
extern void isr22(); 
extern void isr23();
extern void isr24(); 
extern void isr25(); 
extern void isr26(); 
extern void isr27();
extern void isr28(); 
extern void isr29(); 
extern void isr30(); 
extern void isr31();
extern void syscall();
void isrs_install();
//struct regs* fault_handler(struct regs* r);
//#include "io.h"

/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  IRQ
*/


extern void irq0();  
extern void irq1();  
extern void irq2();  
extern void irq3();
extern void irq4();  
extern void irq5(); 
extern void irq6();  
extern void irq7();
extern void irq8();  
extern void irq9();  
extern void irq10(); 
extern void irq11();
extern void irq12(); 
extern void irq13();
extern void irq14(); 
extern void irq15();
void irq_install_handler(int irq, void (*handler)(struct regs* r));
void irq_uninstall_handler(int irq);


/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
              Kernel Calls
*/
#include "tasking.h"
#include "mm/pmm.h"
#include "time.h"
#include "driver/screen.h"
#include "driver/keyboard.h"
//#include "core.h"
#define NUM_SYSCALLS 0x21
void ___sysIdle();
cpu_state *runSyscall(cpu_state *cpu);
void USlistDir(directory *root, directory **result);
void USreadFile(file *f, char **result, size_t size);
void USrunFile(file *name, int *pid);
int writeFile(file *f, char *content);
static void* syscalls[NUM_SYSCALLS] = 
{
	&reboot,
	
	&USrunFile,
	&initTask,
	&idle,
	&USmalloc,		//4
	&free,
	
	&USgetTime,
	&___sysIdle,
	&addTimer,
	&deleteTimer,	//9
	&getTimeStamp,
	
	&USlistDir,
	&USreadFile,
	&___sysIdle,	//writeFile
	&___sysIdle,	//createFile	//14
	&___sysIdle,	//deleteFile
	
	&printChar,
	&clearscreen,
	&changeColor,
	&setFix,		//19
	&___sysIdle,	//setEditorMode,
	&printCharAt,
	
	&USscanChar,
	&USscanInt,
	&scanString,
	&USscanBool,	//25
	&flush,
	&addKeyListener,
	
	
	&___sysIdle,
	&___sysIdle,
	&___sysIdle,//USgetKernel,
	&___sysIdle,		//0x1f
	
	&writeFile
};


/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  GDT
*/
#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS     0x09
#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80
#define GDT_FLAG_4K_GRAN 0x800
#define GDT_FLAG_32_BIT  0x400
#define GDT_ENTRIES      5 
uint64_t gdt[GDT_ENTRIES];

/*xtern*/ void gdt_flush(unsigned long);
void gdt_install();
#include "syscall.c"
#endif
