/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   Floppy
*/
void sendData(uint8_t data)
{
	uint8_t timeout;
	enum FDCREG reg;
	for (timeout = 0; timeout < 200; timeout++)
	{
		if ((inb(reg=MSR) & 0xC0) == 0x80)
		{
			outb(reg=DATA_FIFO, data);
			return;
		}
		sleepTicks(5);  //5ms warten vor dem nächsten Versuch
	}
}

uint8_t read_data()
{
	enum FDCREG reg;
	uint8_t timeout;
	for(timeout = 0; timeout < 200; timeout++)
	{
		if((inb(reg = MSR) & 0xC0) == 0xC0)
			return inb(reg=DATA_FIFO);
		sleepTicks(1);
	}
	return 0;
}
// check interrupt status command
void flpydsk_check_int(ULONG* st0, ULONG* cyl)
{
	sendData(8);
	*st0 = read_data();
	*cyl = read_data();
}

// initialize DMA to use physical address 84k-128k
void initDMA()//uint8_t *byte
{
	outb(0x0a, 0x06);	//mask dma channel 2
	outb(0xd8, 0xFF);	//reset master flip-flop
	outb(0x04, 0x00);
	outb(0x04, 0x10);
	outb(0xd8, 0xFF);   //reset master flip-flop
	outb(0x05, 0xFF);   //count to 0x23FF (number of bytes in a 3.5" floppy disk track: 18*512)
	outb(0x05, 0x23);
	outb(0x81, 0x00);   //external page register = 0
	outb(0x0a, 0x02);   //unmask dma channel 2
}

// prepare the DMA for read transfer
void flpydsk_dma_read()
{
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x56); //single transfer, address increment, autoinit, read, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}

// prepare the DMA for write transfer
void flpydsk_dma_write()
{
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x5A); //single transfer, address increment, autoinit, write, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}

int testMSR()
{
	enum FDCREG reg;
	uint8_t msr = inb(reg=MSR); //Read the MSR
	if ((msr & 0xC0) == 0x80)  //Check if RQM = 1 and DIO = 0
		return true;
	return false;
}
void sendPara( uint8_t para )
{
	enum FDCREG reg;
	while((inb(reg = MSR) & 0xC0) != 0x80 );
	outb(reg = DATA_FIFO, para);
}

void FDCIRQ( struct regs *r)
{
	DEBUG_MSG("Floppy:    FDX-Interrupt!");
}
//Real commands
uint8_t getVersion()
{
	/*
	 * Version cmd
	 *   Version command = 0x10
	 *   No parameter bytes.
	 *   No interrupt.
	 *   First result byte = 0x90 
	 */
	if (!testMSR()) //.................................We must reset and restart
	{	
		resetFDD();
		return getVersion();
	}
	enum FDCCMD cmd; 
	enum FDCREG reg;
	outb(reg=DATA_FIFO, cmd=VERSION); //........................Send Version CMD
	while(testMSR()); //.......................Wait until result/execution phase
	while((inb(reg = MSR) & 0xC0) == 0xC0) //.......................Read results
	{
		uint8_t res = inb(reg = DATA_FIFO);
		return res;
	}
	return false;
}
void resetFDD()
{
	/*
	1. Bit 2 (value = 4) in the DOR: Save the current/"original" value of the DOR, write a 0 to the DOR, wait 4 microseconds, then write the original value (bit 2 is always set) back to the DOR.
   	2. Wait for the resulting IRQ6 (unless you have IRQs turned off in the DOR)
   	3. If (and only if) drive polling mode is turned on, send 4 Sense Interrupt commands (required).
   	4. If your OS/driver never sent a Lock command, then you probably need to send a new Configure command (the fifo settings were lost in the reset).
   	5. Do a Drive Select procedure for the next drive to be accessed. */

	enum FDCREG reg;
	
	uint8_t dor = inb( reg=DOR );
	outb( reg=DOR, 0 );
	sleepTicks(1);
	
	//DEBUG_MSG("DOR = %d", dor);
	dor |= 0x04; //.........................................To set the RESET bit
	//dor &= 0xFB; //To set the  DMA  bit
	//DEBUG_MSG("DOR = %d", dor);
	///modify dor
	outb( reg=DOR, dor );
	
	if(fdc.drive_polling_mode);
	if (!fdc.lock)
	{
		configure(fdc.drive_polling_mode, fdc.FIFO, fdc.threshold, fdc.implied_seek, fdc.precompensation);
	}
}



void setMotor(int state)
{
	if ( fdc.drives[ fdc.acDrive ].motor != state )
	{
		enum FDCREG reg;
		uint8_t dor = inb( reg=DOR );
	
		dor |= state << (4+fdc.acDrive);
	
		outb( reg=DOR, dor); 

		//sleepTicks(5); TODO Here must be a sleep
	}
	fdc.drives[ fdc.acDrive ].motor = state;
}

void selectFDD(int drive)
{
	/*
	1. Send the correct Datarate setting to CCR. Usually this is a 0 (1.44MB floppy drive).
	2. If the newly selected drive is a different type than the previously selected drive (or changing from PIO to DMA mode), send a new Specify command.
	3. Set the "drive select" bits (and the other bitflags) in DOR properly, including possibly turning on the Motor bit for the drive (if it will be accessed soon). */
	
	enum FDCREG reg;
	outb(reg = CCR, 0);
	uint8_t dor = inb(reg = DOR);
	outb(reg = DOR, dor|drive);	
	fdc.acDrive = drive;
	setMotor(true);
}
void recalibrateFDC()
{
	setMotor(true);
	enum FDCREG reg;
	enum FDCCMD cmd;
	/*
 	 * Recalibrate command = 0x7
 	 * First parameter byte = drive number = 0 to 3.
 	 * No result bytes.
	 * The interrupt may take up to 3 seconds to arrive, so use a long timeout. 
	 */
	if (!testMSR())
	{	//We must reset and restart. Something wrong...
		resetFDD();
		recalibrateFDC();
	}
	
	outb( reg=DATA_FIFO, cmd=RECALIBRATE );
	sendPara( fdc.acDrive );
	sleepTicks(10); //TODO Better sleep here
}

void configure(int drive_polling_mode, int FIFO, int threshold, int implied_seek, int precompensation)
{
	/*
	Configure:
	    * Configure command = 0x13
	    * First parameter byte = 0
	    * Second parameter byte = (implied seek enable << 6) | (fifo DISable << 5) | (drive polling mode enable << 4) | thresh_val (= threshold - 1)
	    * Third parameter byte = precomp_val = 0
	    * No result bytes.
	    * No interrupt. 
	*/
	
	if ( !testMSR() )
	{	//We must reset and restart. Something wrong...
		resetFDD();
		configure( drive_polling_mode,  FIFO, threshold, implied_seek, precompensation);
	}
	enum FDCCMD cmd; enum FDCREG reg;
	//DEBUG_MSG("Send CMD...");
	outb( reg=DATA_FIFO, cmd=CONFIGURE);
	
	//Send parameters
	sendPara(0);
	sendPara( (implied_seek << 6) | ( (!FIFO) << 5) | (drive_polling_mode << 4) | (threshold - 1) ) ; 
	sendPara( precompensation );
	
	fdc.drive_polling_mode = drive_polling_mode;
	fdc.FIFO = FIFO;
	fdc.threshold = threshold;
	fdc.implied_seek = implied_seek;
	fdc.precompensation = precompensation;
}

uint8_t lock( int locking )
{
	/*
	    * Lock command = 0x94
	    * or Unlock command = 0x14
	    * No parameter bytes.
	    * No interrupt.
	    * First result byte = lock bit << 4 */

	if ( !testMSR() )
	{	//We must reset and restart. Something wrong...
		resetFDD();
		lock( locking );
	}
	enum FDCCMD cmd; enum FDCREG reg;
	if ( locking )
		outb( reg=DATA_FIFO, cmd=  LOCK);
	else
		outb( reg=DATA_FIFO, cmd=UNLOCK);
	
	while( !testMSR() );
	while( (inb(reg=MSR) & 0xC0) == 0xC0) //Read results
	{
		return inb(reg=DATA_FIFO) >> 4; //res = result
	}
	fdc.lock = locking;
	return 0;
}

void fddIRQhandler( struct regs * r )
{
}


void initFDD()
{
	irq_install_handler(6, fddIRQhandler);
	DEBUG_MSG("Floppy:   Check FDC compatibility...");
	if (getVersion() == 0x90)
    	dbg(true); //We have 82077AA FDC
    else
    {
        dbg(false);
        return;
    }

	/*
	 * Config Values:
     *   drive polling mode off
	 *   FIFO on
	 *   threshold = 8
	 *   implied seek on
	 *   precompensation 0
     */
    DEBUG_MSG("Floppy:   Configureing FDC...");
    configure(0, 1, 8, 1, 0);
    dbg(true);
    //Send a lock command to not reconfigure the FDC after reset
    DEBUG_MSG("Floppy:   Locking FDC Configuration...");
    lock(1);
    dbg(true);
    DEBUG_MSG("Floppy:   Reset FDC...");
    resetFDD();
    dbg(true);
    DEBUG_MSG("Floppy:   Select FDD...");
    selectFDD(0);
    dbg(true);
    DEBUG_MSG("Floppy:   Recalibrate FDC");
    recalibrateFDC();
    dbg(true);
	initDMA();
}

int seek( ULONG cyl, ULONG head )
{
	ULONG st0, cyl0;
	int i;
	for(i = 0; i < 10; i++)
	{
		// send the command
		sendData(0xF); //FDC_CMD_SEEK
		sendData((head) << 2 | fdc.acDrive);
		sendData(cyl);

		//flpydsk_wait_irq();
		//sleepTicks(5); //TODO we must sleep
		flpydsk_check_int(&st0,&cyl0);

		if (cyl0 == cyl) // found the cylinder?
			return 0;
	}
	return -1;
}

// write a sector
int writeSector (int sectorLBA)
{
	acTask->writing = true;
	
	int head   = (sectorLBA % (SECTORS_PER_TRACK * 2)) / SECTORS_PER_TRACK;
	int track  =  sectorLBA / (SECTORS_PER_TRACK * 2);
	int sector =  sectorLBA %  SECTORS_PER_TRACK + 1;
	
	// turn motor on and seek to track
	setMotor(true);
	if(seek (track, head)) return -2;
	// write sector
	
	ULONG st0, cyl;
	flpydsk_dma_write(); // set the DMA for write transfer

	sendData(0x05 | 0x40);  // write a sector
	sendData(head << 2 | fdc.acDrive);
	sendData(track);
	sendData(head);
	sendData(sector);
	sendData(2); //FLPYDSK_SECTOR_DTL_512
	sendData(((sector + 1) >= SECTORS_PER_TRACK) ? SECTORS_PER_TRACK : sector + 1);
	sendData(27); //FLPYDSK_GAP3_LENGTH_3_5
	sendData(0xFF);

	//flpydsk_wait_irq();
	//sleepTicks(5); //FIXME WAIT HERE !!!!
	int j;
	for(j = 0; j < 7; j++)
		read_data();      // read status info
	flpydsk_check_int(&st0, &cyl); // let FDC know we handled interrupt


	//and turn motor off
	
	setMotor(false);
	acTask->writing = false;
	return 0;
}


int readSector(int sectorLBA)
{
	asm("sti");
	acTask->writing = true;
	//int head = 0, track = 0, sector = 1;

	int head   = (sectorLBA % (SECTORS_PER_TRACK * 2)) / SECTORS_PER_TRACK;
	int track  =  sectorLBA / (SECTORS_PER_TRACK * 2);
	int sector =  sectorLBA %  SECTORS_PER_TRACK + 1;	
	
	// turn motor on and seek to track
	setMotor(true);
	if(seek(track, head)) return -2;
	ULONG st0, cyl;
	flpydsk_dma_read();  // set the DMA for read transfer

	sendData(0x06 | 0x40); // read a sector
	sendData(head << 2 | fdc.acDrive);
	sendData(track);
	sendData(head);
	sendData(sector);
	sendData(2);
	sendData(((sector + 1) >= SECTORS_PER_TRACK) ? SECTORS_PER_TRACK : sector + 1);
	sendData(27);
	sendData(0xFF);

	//flpydsk_wait_irq();
	int j;
	for(j = 0; j < 7; j++)
		read_data();      // read status info
	flpydsk_check_int(&st0, &cyl); // let FDC know we handled interrupt

	setMotor(false);
	acTask->writing = false;
	return 0;
}



