/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                Hardware
*/

unsigned inportb(unsigned port) //Henkessoft
{
    unsigned ret_val;
    asm volatile ("inb %w1,%b0"    : "=a"(ret_val)    : "d"(port));
    return ret_val; 
}
void outportb(unsigned port, unsigned val) //Henkessoft
{
    asm volatile ("outb %b0,%w1" : : "a"(val), "d"(port));
}

void reboot()
{
	DEBUG_MSG("SysCall:  Force Reboot...");
	int temp; // A temporary int for storing keyboard info. The keyboard is used to reboot
	do //flush the keyboard controller
	{
		temp = inportb(0x64);
		if(temp & 1)
			inportb(0x60);
	}
	while (temp & 2);
	// Reboot
	dbg(true);
	outportb(0x64, 0xFE);
}

/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   IDT
*/

static void idt_load(){
	DEBUG_MSG("IDT:      Load...");
	asm volatile("lidt %0" : "=m" (idt_register)); 
    dbg(true);
}
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = (base        & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     =   sel;
    idt[num].always0 =     0;
    idt[num].flags   = flags;
}

void idt_install()
{
	DEBUG_MSG("IDT:      Install...");
    // Sets the special IDT pointer up
    idt_register.limit = (sizeof (struct idt_entry) * 256)-1;
    idt_register.base  = (unsigned int) &idt;
    kmemset(&idt, 0, sizeof(struct idt_entry) * 256);
    dbg(true);
    idt_load();
}

/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   ISR
*/
/* Set the first 32 entries in the IDT to the first 32 ISRs. Access flag is set to 0x8E: present, ring 0,
*  lower 5 bits set to the required '14' (hexadecimal '0x0E'). */
void isrs_install()
{
    idt_set_gate( 0, (unsigned) isr0, 0x08, 0x8E);
    idt_set_gate( 1, (unsigned) isr1, 0x08, 0x8E);
    idt_set_gate( 2, (unsigned) isr2, 0x08, 0x8E);    
    idt_set_gate( 3, (unsigned) isr3, 0x08, 0x8E);
    idt_set_gate( 4, (unsigned) isr4, 0x08, 0x8E);    
    idt_set_gate( 5, (unsigned) isr5, 0x08, 0x8E);
    idt_set_gate( 6, (unsigned) isr6, 0x08, 0x8E);    
    idt_set_gate( 7, (unsigned) isr7, 0x08, 0x8E);
    idt_set_gate( 8, (unsigned) isr8, 0x08, 0x8E);    
    idt_set_gate( 9, (unsigned) isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);    
    idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);    
    idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);    
    idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);    
    idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);    
    idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);    
    idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);    
    idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);    
    idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);    
    idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);    
    idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);    
    idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
}


/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  IRQ
*/


/* Array of function pointers handling custom IRQ handlers for a given IRQ */
void* irq_routines[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Implement a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler) (struct regs* r))
{
	irq_routines[irq] = handler;
}

/* Clear the custom IRQ handler */
void irq_uninstall_handler(int irq)
{
	irq_routines[irq] = 0;
}

cpu_state * schedule(cpu_state* esp);
//unsigned int irq_handler(unsigned int esp)
cpu_state * irq_handler(cpu_state* cpu)
{
	void (*handler) (struct regs* r);

	// Find out if we have a custom handler to run for this IRQ, and then finally, run it 
	handler = irq_routines[cpu->int_no - 32];
	if (cpu->int_no-32 == 9)
	{
		cpu = runSyscall(cpu);
		return cpu;
	}
	
	//if (cpu->int_no-32 != 0 && cpu->int_no-32 != 1) kprintf("Getting IRQ which is NOT Keyboard or time %d", cpu->int_no-32);
	if (handler) handler(cpu);
	if (cpu->int_no-32 == 0)
		cpu = schedule(cpu); 
	
	if (cpu->int_no >= 40) outportb(0xA0, 0x20);
	outportb(0x20, 0x20);
	return cpu;	
}

#define PIC_MASTER_COMMAND	0x20
#define PIC_MASTER_DATA		0x21
#define PIC_MASTER_IMR		0x21
#define PIC_SLAVE_COMMAND	0xA0
#define PIC_SLAVE_DATA		0xA1
#define PIC_SLAVE_IMR		0xA1
#define IRQ_TO_IDT			32 
/* Remap: IRQ0 to IRQ15 have to be remapped to IDT entries 32 to 47 */

/**
 * IRQ_Remap
 * @source LowLevel.org
 */
void irq_remap()
{
	DEBUG_MSG("IRQ:      Remap...");
	outb(PIC_MASTER_COMMAND, 0x11);
	outb(PIC_MASTER_COMMAND, 0x11);
	outb(PIC_MASTER_DATA, IRQ_TO_IDT);
	outb(PIC_SLAVE_DATA, IRQ_TO_IDT+8);
	outb(PIC_MASTER_DATA, 0x04);
	outb(PIC_SLAVE_DATA, 2);
	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);
	uint16_t mask = 0;//In this value is a bit for every IRQ (IRQ0 is Bit 0 and so on). Is the bit setted (1) the IRQ is mask meaning the CPU doesn't act. A deleted Bit (0) means the IRQ is activ. 
	outb(PIC_MASTER_IMR, (uint8_t) mask);
	outb(PIC_SLAVE_IMR, (uint8_t) mask>>8);
	dbg(true);
}

/* After remap of the interrupt controllers the appropriate ISRs are connected to the correct entries in the IDT. */
void irq_install()
{
    irq_remap();
	DEBUG_MSG("INT:      Install...");
    idt_set_gate(32, (unsigned) irq0,  0x08, 0x8E);   idt_set_gate(33, (unsigned) irq1,  0x08, 0x8E);
    idt_set_gate(34, (unsigned) irq2,  0x08, 0x8E);   idt_set_gate(35, (unsigned) irq3,  0x08, 0x8E);
    idt_set_gate(36, (unsigned) irq4,  0x08, 0x8E);   idt_set_gate(37, (unsigned) irq5,  0x08, 0x8E);
    idt_set_gate(38, (unsigned) irq6,  0x08, 0x8E);   idt_set_gate(39, (unsigned) irq7,  0x08, 0x8E);
    idt_set_gate(40, (unsigned) irq8,  0x08, 0x8E);   idt_set_gate(41, (unsigned) irq9,  0x08, 0x8E);
    idt_set_gate(42, (unsigned) irq10, 0x08, 0x8E);   idt_set_gate(43, (unsigned) irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned) irq12, 0x08, 0x8E);   idt_set_gate(45, (unsigned) irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned) irq14, 0x08, 0x8E);   idt_set_gate(47, (unsigned) irq15, 0x08, 0x8E);
    dbg(true);
}

/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
              Kernel Calls

*/
void ___sysIdle()
{}

cpu_state *runSyscall(cpu_state *cpu)
{
	if(cpu->eax >= NUM_SYSCALLS)
		return cpu;
	if (cpu->eax == 100)
	{
		saveCPU( cpu );
		return cpu;
	}
	void* addr = syscalls[cpu->eax]; // Get the required syscall location.
	// Our function can have max. 5 parameters
	int ret;
	asm volatile (" \
	push %1; \
	push %2; \
	push %3; \
	push %4; \
	push %5; \
	call *%6; \
	add $20, %%esp; \
	" : "=a" (ret) : "r" (cpu->edi), "r" (cpu->esi), "r" (cpu->edx), "r" (cpu->ecx), "r" (cpu->ebx), "r" (addr));
	cpu->eax = ret;
	return cpu;
}
/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  GDT
 * Global Descriptor Table
 * Methods: gdt_install() Installs the GDT
 */

static void set_entry(int i, unsigned int base, unsigned int limit, int flags)
{
	gdt[i] = limit & 0xffffLL;
	gdt[i] |= (base & 0xffffffLL) << 16;
	gdt[i] |= (flags & 0xffLL) << 40;
	gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
	gdt[i] |= ((flags >> 8 )& 0xffLL) << 52;
	gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}
void gdt_install()
{
	DEBUG_MSG("GDT:      Install...");
	struct gdt_ptr
	{
		unsigned short limit;
		void * base;
	}__attribute__((packed));
	struct gdt_ptr gdtp = {
		.limit = (sizeof(uint64_t) * GDT_ENTRIES)-1,
		.base  =  gdt,
	};
	set_entry(0, 0             , 0           , 0);
	set_entry(1, 0             , 0xfffff     , GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT  | GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT);
	set_entry(2, 0             , 0xfffff     , GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT  | GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT);
	set_entry(3, 0             , 0xfffff     , GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT  | GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
	set_entry(4, 0             , 0xfffff     , GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT  | GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
	set_entry(5, (uint32_t) tss, sizeof(tss), GDT_FLAG_TSS     | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
	//gdt_flush((unsigned long) &gdtp);


	asm volatile(	"lgdt %0" : : "m" (gdtp));
	asm volatile(	"mov %ax, 0x10\n\t" 
			"mov %ds, %ax\n\t"
			"mov %es, %ax\n\t"
			"mov %fs, %ax\n\t"
			"mov %gs, %ax\n\t"
			"mov %ss, %ax\n\t"
			"jmp .flush\n\t"
			".flush:");


	dbg(true);
}



#include "datatypes.h"
