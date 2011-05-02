/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                    ELF
*/

/**
 * Intializes an ELF32-TASK
 * TODO: Check Length of image
 * TODO: US-Mode
 * @param image The image
 * @param pri Priority
 */
void* initELF(void* image, int pri, size_t size, char *args)
{
	/*
	* TODO Wir muessen eigentlich die Laenge vom Image pruefen, damit wir bei
	* korrupten ELF-Dateien nicht ueber das Dateiende hinauslesen.
	*/
	DEBUG_MSG("ELF:      Init ELF32-File...");
	struct elf_header* header = image;
	struct elf_program_header* ph;
	int i;

	/* Ist es ueberhaupt eine ELF-Datei? */
	if (header->magic != ELF_MAGIC) {
		dbg(false);
		kprintf("Keine gueltige ELF-Magic!\n");
		return 0;
	}

	/*
	* Alle Program Header durchgehen und den Speicher an die passende Stelle
	* kopieren.
	*
	* TODO Wir erlauben der ELF-Datei hier, jeden beliebigen Speicher zu
	* ueberschreiben, einschliesslich dem Kernel selbst.
	*/
	ph = (struct elf_program_header*) (((char*) image) + header->ph_offset);
	for (i = 0; i < 2/*header->ph_entry_count*/; i++, ph++) {
		void* dest = (void*) ph->virt_addr;
		void* src = ((char*) image) + ph->offset;

		/* Nur Program Header vom Typ LOAD laden */
		if (ph->type != 1)
			continue;

		kmemset(dest, 0, ph->mem_size);
		kmemcpy(dest, src, ph->file_size);
	}
	

	//size /= PAGE_SIZE;

	
	pageDir_t page = vmmCreateContext();
	void *paddr = pmm_malloc(PAGE_SIZE);
		
	kmemset((void*) page, 0, PAGE_SIZE);
	
	for(i=0; i<size;i +=PAGE_SIZE)
	{
		vmmMapPage( page, (void*) 0xfffff000+i, paddr+i, USER_PRV ); //0xfffff000
	}
	
	vmmActivateContext( page );

	
	
	initTask( (void *)header->entry, pri, page, args);
	dbg(true);
	return (void*) header->entry;
}
