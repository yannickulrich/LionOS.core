/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
               Multitasking
*/
#include "mm/vmm.h"
void idle()
{
	int sTime = getTime();
	while(sTime == getTime())
		asm("nop");
}

/*
 * Jeder Task braucht seinen eigenen Stack, auf dem er beliebig arbeiten kann,
 * ohne dass ihm andere Tasks Dinge ueberschreiben. Ausserdem braucht ein Task
 * einen Einsprungspunkt.
 */
int initTask(void* entry, int priority, pageDir_t page, char *args)
{
	
	/*int i, argc;
	for(i=0; args[i]!='\0';i++)
	{
		if (args[i]==' ') argc++;
	}
	char *argl[argc];
	
	int lastI = 0;
	int argID = 0;
	for(i=0;  args[i]!='\0';i++)
	{
		if (args[i]==' ')
		{
			argl[argID]=pmm_malloc(i-lastI);
			argID++;
		}
	}
	//TODO make this more efficent
	
	argID = 0;
	int inargID=0;
	for(i=0; args[i]!='\0';i++)
	{
		if (args[i]==' ')
		{
			argID++;
			inargID=0;
		}
		argl[argID][inargID]=args[i];
	}*/
	
	
	uint8_t *    stack  = (uint8_t *) pmm_malloc(sizeof(uint8_t[4096]));
	uint8_t *userstack  = (uint8_t *) pmm_malloc(sizeof(uint8_t[4096]));
	
	//userstack[4091]=10;
	/*
	* CPU-Zustand fuer den neuen Task festlegen
	*/

	cpu_state new_state = {
		.eax = (int) args,
		.ebx = 0, 
		.ecx = 0,
		.edx = 0,
		.esi = 0,
		.edi = 0,
		.ebp = 0,
		//.esp = unbenutzt (kein Ring-Wechsel)
		.esp = (uint32_t) userstack + 4096,
		.eip = (uint32_t) entry,

		// Ring-0-Segmentregister 
		.cs  = 0x08,

		// IRQs einschalten (IF = 1)
		.eflags = 0x202,
	};

	/*
	* Den angelegten CPU-Zustand auf den Stack des Tasks kopieren, damit es am
	* Ende so aussieht als waere der Task durch einen Interrupt unterbrochen
	* worden. So kann man dem Interrupthandler den neuen Task unterschieben
	* und er stellt einfach den neuen Prozessorzustand "wieder her".
	*/
	cpu_state* state = (void*) (stack + 4096 - sizeof(new_state));
	
	*state = new_state;
	
	
	
	
	
	if (firstTask == NIL)
	{
		firstTask = pmm_malloc(sizeof(task));
		firstTask->next = NIL;
		firstTask->pid = 0;
		lastTask++;
		firstTask->esp = state;
		firstTask->killNext = 0;
		firstTask->context = page;
		firstTask->cmdline = args;
		return 0;
	}
	
	task *t = firstTask;
	while(t->next != NIL)
		t = t->next;
	if (t->next != NIL)
	{
		t = t->next;
	}
	t->next = pmm_malloc( sizeof(task) );
	t->pid = lastTask;
	t->cmdline = args;
	lastTask++;
	t->killNext = 0;
	t->esp = state;
	t->context = page;
	currentPID = 0;
	return t->pid;
}

void saveCPU(cpu_state* cpu)
{
	task *t = firstTask;
	while(t->next != NIL)
	{
		if (t->pid == currentPID)
			break;
		else t = t->next;
	}
	t->esp = cpu;
}

cpu_state * forceTaskSwitch(cpu_state *cpu)
{
	if ((int) cpu == -1)
	{
		kprintf("Force Switch without saving\n");
		//We have a seriouse problem...
		task *t = firstTask;
		while(t->next != NIL)
		{
			if (t->pid == currentPID)
			{
				kprintf("Force Quit of Task %d\n", t->pid);
				break;
			}
			t = t->next;
		}
		t->next = t->next->next;
		free(t->next);
	}
	else
	{
		task *t = firstTask;
		while(t->next != NIL)
		{
			if (t->pid == currentPID)
				break;
			else t = t->next;
		}
		t->esp = cpu;
	}
	
	currentPID++;
	currentPID %= lastTask+1;

	task *t = firstTask;
	while(t->next != NIL)
	{
		if (t->pid == currentPID) break;
		else t = t->next;
	}

	// Prozessorzustand des neuen Tasks aktivieren
	cpu = t->esp;
	acTask = t;
	tss[1] = (uint32_t) (cpu + 1);
	
	return cpu;
}

cpu_state* schedule(cpu_state* cpu)
{
	/*
	* Wenn schon ein Task laeuft, Zustand sichern. Wenn nicht, springen wir
	* gerade zum ersten Mal in einen Task. Diesen Prozessorzustand brauchen
	* wir spaeter nicht wieder.
	*/
	if (killNextTask)
	{
		killNextTask = 0;
		task *t = firstTask;
		while(t->next != NIL)
		{
			if (t->pid == currentPID-1) break;
			t = t->next;
		}
		t->next = t->next->next;
		free(t->next);
		
	}
	if (lastTask == 0)
		return cpu;
	if (currentPID >= 0 && firstTask != NIL) {
		task *t = firstTask;
		while(t->next != NIL)
		{
			if (t->pid == currentPID) break;
			else t = t->next;
		}
		t->esp = cpu;
	}
	
	// Naechsten Task auswaehlen. Wenn alle durch sind, geht es von vorne los
	currentPID++;
	currentPID %= lastTask+1;

	task *t = firstTask;
	while(t->next != NIL)
	{
		if (t->pid == currentPID) break;
		else t = t->next;
	}

	// Prozessorzustand des neuen Tasks aktivieren
	cpu_state * newCPU = t->esp;
	acTask = t;
	tss[1] = (uint32_t) (newCPU + 1);

	if (cpu != newCPU && acTask->context != NIL) {
		//asm volatile("mov %%cr3, %0" : : "r" (acTask->context));
		vmmActivateContext(acTask->context);
	}
	
	return newCPU;
}

void startTasking()
{
	DEBUG_MSG("MM:       Start Multitasking...");
	asm("sti");
	dbg(true);
}
