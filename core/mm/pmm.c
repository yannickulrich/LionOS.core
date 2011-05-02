/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   PMM
*/

void free(void* page)
{
	size_t bitmap_index = (size_t) page / PAGE_SIZE / PMM_BITS_PER_ELEMENT;
	//pc("[00.000013] +pmmbitmap[%d] |= %d", bitmap_index, (1 << (((size_t) page / PAGE_SIZE) & (PMM_BITS_PER_ELEMENT - 1))));
	pmmbitmap[bitmap_index] |= 1 << (((size_t) page / PAGE_SIZE) & (PMM_BITS_PER_ELEMENT - 1));
	
}
void pmm_mark_used(void* page)
{
	size_t bitmap_index = (size_t) page / PAGE_SIZE / PMM_BITS_PER_ELEMENT;
	pmmbitmap[bitmap_index] &= ~(1 << (((size_t) page / PAGE_SIZE) & (PMM_BITS_PER_ELEMENT - 1)));
	
}

void pmminit(struct multiboot_info *mb_info)
{	
	DG("PMM:      Init...");
	pc("[00.000027] +kernel_start = %d", &kernel_start);
	uint16_t addr = (uint16_t) &kernel_start;
	pc("[00.000029] +for addr < kernel_end do useMark");
	while (addr < (uint16_t) &kernel_end) {
		pmm_mark_used((void*) addr);
		addr += 0x1000;
	}

	struct multiboot_mmap* mmap = mb_info->mb_mmap_addr;
	struct multiboot_mmap* mmap_end = (void*) ((uint16_t) mb_info->mb_mmap_addr + mb_info->mb_mmap_length);

	struct multiboot_module *mod = mb_info->mb_mods_addr;
	pc("[00.000039] +useMark multibootModule");
	pmm_mark_used(mod);
	int i;
	void *Maddr;
	for (i = 0; i < mb_info->mb_mods_count; i++) {
		Maddr = mod[i].start;
		while (Maddr < mod[i].end) {
			pmm_mark_used((void*) Maddr);
			Maddr += 0x1000;
		}
	}

	while (mmap < mmap_end) {
		// Der Speicherbereich ist frei, entsprechend markieren
		uint16_t addr = mmap->base;
		uint16_t end_addr = addr + mmap->length;
		if (mmap->type == 1) {
			// Der Speicherbereich ist frei, entsprechend markieren
			addr = mmap->base;
			end_addr = addr + mmap->length;

			while (addr < end_addr) {
				free((void*) addr);
				addr += 0x1000;
			}
		}
		mmap++;
	}
	dbg(true);
}

void* pmm_prepare_alloc(size_t num) //Gives number of pages
{
	size_t i,j, found = 0;
	void * pages = NIL;
	for(i=0;i<BITMAP_SIZE;i++)
	{
		if (pmmbitmap[i] == 0) continue;//Everything used...
		if (pmmbitmap[i] == ~0x0 ) //Everything free!
		{
			if (found == 0)
				pages = (void*)(i * PMM_BITS_PER_ELEMENT * PAGE_SIZE);
			found += PMM_BITS_PER_ELEMENT;
		}
		else
		{
			for(j=0;j<PMM_BITS_PER_ELEMENT;j++)
			{
				if ( pmmbitmap[i] & ( 1 << j ) )
				{
					//Cool, page is free!
					if (found == 0)
					{
						pages = (void*)( (i * PMM_BITS_PER_ELEMENT + j) * PAGE_SIZE);
					}
					found++;
					if (found > num)
					{
						//Cool, we get enough.
						return pages;
					}
				}
				else
				{
					found = 0;
				}
			}
		}
		if (found > num)
			return pages;
	}
	return (void*) -1;
}

void* pmm_malloc( size_t size )
{
	size_t page_count = size / PAGE_SIZE;
	if((size % PAGE_SIZE) != 0)
		page_count++;
	void * pages = pmm_prepare_alloc(page_count);
	
	if (pages == (void*) -1) panic(PF);
	pmm_mark_used(pages);
	return pages;
}

void USmalloc(size_t size, void** p)
{
	*p = pmm_malloc(size);
}

void* kmemcpy(void* dest, const void* src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}
void* kmemset(void* dest, char val, size_t count)
{
    char* temp = (char*) dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}
