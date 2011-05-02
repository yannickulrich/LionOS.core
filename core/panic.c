/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                  Panic
*/
#ifndef PANICC
#define PANICC

/**
 * @visibility Kernel
 */
struct regs* fault_handler(struct regs* r)
{
    if (r->int_no < 32)
    {
    	panic(r->int_no);
		while(1); asm("cli"); asm("hlt");
			kprintf("ErNo: %d, ErCo: %d, ESP: %d, EIP: %d", r->int_no, r->err_code, r->esp, r->eip);
		return forceTaskSwitch( (cpu_state*) -1 );
    }
	return r;
}

/**
 * Paints a bluescreen and halts the system.
 * @param reasen The Error number
 */
void panic(int reason)
{	
	changeColor((char) 0x1, (char) 0xF);
	setFix(0);
	clearscreen();
	writingType = INSDEL;
	int i, j;
	for(i = 0; i < 80; i++)
		printChar((char) 0);
	printChar(' ');
	printChar((char) 219); 
	for(i = 0; i < 76; i++) 
		printChar((char) 223);
	printChar((char) 219); 
	printChar(' ');
	for(j = 0; j < 5; j++)
	{
		printChar(' ');
		printChar((char) 219); 
		for(i = 0; i < 76; i++) printChar(' ');
		printChar((char) 219); 
		printChar(' ');
	}
	printChar(' ');
	printChar((char) 219);
	for(i = 0; i < 30; i++) 
		printChar(' ');
	kprintf("KERNEL PANIC!!!");
	for(i = 0; i <31; i++) 
		printChar( ' ');
	printChar((char) 219); 
	printChar(' ');
	printChar(' ');
	printChar((char) 219); 
	for(i = 0; i < 76; i++) 
		printChar( ' ');
	printChar((char) 219); 
	printChar(' ');
	printChar(' ');
	printChar((char) 219); 
	for(i = 0; i < 30; i++)
		printChar( ' ');
	kprintf("EXEPTION THROWN");
	for(i = 0; i < 31; i++) 
		printChar(' ');
	printChar((char) 219);
	printChar(' ');
	printChar(' ');
	printChar((char) 219); 
	for(i=0; i<76;i++)
		printChar( ' ');
	printChar((char) 219); 
	printChar(' ');
	printChar(' ');
	printChar((char) 219); 
	for(i=0; i<35;i++) 
		printChar(' ');
	kprintf("ERROR");
	for(i = 0; i<34;i++)
		printChar(' ');
	printChar((char) 219); 
	printChar(' ');
	
	
	printChar(' ');
	printChar( (char) 219 ); 
	for(i=0; i<36;i++) printChar( ' ');
	
	if (reason < 10)
		printChar('0');
	kprintf("%d", reason);
	for(i=0; i<38;i++) { printChar( ' '); }
	printChar( (char) 219 ); 
	printChar(' ');

	for(j=0; j<10; j++)
	{
		printChar(' ');
		printChar( (char) 219 ); 
		for(i=0; i<76;i++) { printChar( ' '); }
		printChar( (char) 219 ); 
		printChar(' ');
	}
	printChar(' ');
	printChar( (char) 219 ); 
	for(i=0; i<76;i++) { printChar( (char) 220); }
	printChar( (char) 219 ); 
	printChar(' ');
	for(i=0; i<80;i++) { printChar( (char) 0); }
	/*
	+--------------------------------------------------------+
	|                                                        |
	|                                                        |
	|                                                        |
	|                   KERNEL PANIC!!!                      |
	|                                                        |
	|                  EXEPTION THROWN                       |
	|                                                        |
	|                       ERNO:                            |
	|                       0x..                             |
	|                                                        |
	|                                                        |
	|                                                        |
	|                                                        |
	+--------------------------------------------------------+
	*/
	
	//while(1);
}
#endif
