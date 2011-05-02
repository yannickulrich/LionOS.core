/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   VMM
*/

void vmmInit()
{
	
	int i;

	kernelPageDir = vmmCreateContext();

	for(i=0x1000; i<0x1000*0x400;i +=0x1000)
	{
		vmmMapPage(kernelPageDir, (void*)i, (void*)i, KERNEL_PRV);	
	}	

	for(i=0xB8000; i<0xBFFFF;i +=0x1000)
	{
		vmmMapPage(kernelPageDir, (void*)i, (void*)i, KERNEL_PRV);	
	}	

	vmmActivateContext( kernelPageDir);
	
	uint32_t cr0;
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= ( 1 << 31 );
	
	asm volatile("mov %%cr0, %0" : : "r" (cr0) );
	
}

pageDir_t  vmmCreateContext()
{
	pageDir_t context = pmm_malloc( sizeof(pageDir_t) );
	int i;
	//context->pageDir_t = pmm_malloc( sizeof( uint32_t) );

	for( i= 0; i < 1024; i++ )
	{
		context/*->pageDir_t[i]*/ = 0;
	}
	return context;
}

void vmmActivateContext( pageDir_t context)
{
	//asm volatile("mov %0, %%cr3" : : "r" (context/*->pageDir_t*/) );
	asm volatile("mov %%cr3, %0" : : "r" (acTask->context));
}

void vmmMapPage (pageDir_t context, void *vaddr, void *paddr, uint8_t flags )
{
	pageTable_t table;
	//int flags = PTEpRESENT | ( (prv == KERNEL_PRV) ? 0 : PTEwRITE );
	//pagenumber = vaddr / 4096
	//pte = paddr / 0x3
	uint32_t vpage = (uint32_t)vaddr / 0x1000;
	if (vaddr == NIL)
		kprintf("Error: vaddr is NULL");
	
	if (( context[vpage/0x400] & PTEpRESENT ) == 0 ) //Page unpresent?
	{//Yes
		table = pmm_malloc(sizeof(unsigned long) );
		context[vpage/0x400] = ( (uint32_t) table) | flags | 0x004;
		kmemset(table, 0, 0x1000);
	}
	else
	{
		if( context == kernelPageDir )
		{
			//Kernel
			table = (pageTable_t) ( 0x3FC00000 + (sizeof(uint32_t)*vpage & ~0xFFF) ); // "& ~0xFFF" keeps flags, "0x3FC00000" = base
			
			
		}
		else
		{
			table = (pageTable_t) (context[vpage/0x400] & ~0xFFF);
			//Table isnt part of kernel
			//If you use it direct => Page Fault
			//First: map in kernel

			void * krnTable = pmm_malloc(PAGE_SIZE);
			vmmMapPage(kernelPageDir, krnTable, (void *) table, KERNEL_PRV );
			table = krnTable;
		}
	}

	table[vpage % 0x400] = ( (uint32_t) paddr) | flags;

	asm volatile("invlpg %0" : : "m" (*(char*)vaddr));

	
}



