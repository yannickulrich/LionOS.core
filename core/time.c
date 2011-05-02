/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                 Timer
*/
void installTimer()
{
	DEBUG_MSG("Time:     Init...");
	irq_install_handler(0, timer_handler);
	ticks = 0;
	int i;
	for(i = 0; i < TIMER_SIZE; i++)
		timer_routines[i] = 0;
	pit_init(10);
	dbg(true);
}
int getTime()
{
	return ticks;
}
void USgetTime(int *time)
{
	*time = ticks;
}
int addTimer(void (*handler))
{
	DEBUG_MSG("Time:     Adding Timer...");
	int i;
	for(i = 0; i < TIMER_SIZE; i++)
	{
		if (timer_routines[i] == 0)
		{
			timer_routines[i] = handler;
			dbg(true);
			return i;
		}
	}
	dbg(false);
	return -1;
}

void USaddTimer( void (*handler), int *id )
{
	DEBUG_MSG("Time:     Adding Timer...");
	int i;
	for(i = 0; i < TIMER_SIZE; i++)
	{
		if (timer_routines[i] == 0)
		{
			timer_routines[i] = handler;
			dbg(true);
			*id = i;
			return;
		}
	}
	dbg(false);
	*id = -1;
}

void deleteTimer(int id)
{
	timer_routines[id] = 0;
}
void timer_handler(struct regs* r)
{
	ticks++;
	int i;
	void (*handler)();
	for(i = 0; i < TIMER_SIZE; i++)
	{
		if (timer_routines[i] != 0)
		{
			handler = timer_routines[i];
			if (handler) handler();
		}
	}
}

void pit_init(int freq)
{
   freq = 1193180 / freq;
   outb(0x43, 0x34);
   outb(0x40,freq & 0xFF);
   outb(0x40,freq >> 8);
}

/**
 * Sleeps a while.
 * @param ticks The count of ticks to sleep.
 */
void sleepTicks(long tticks)
{
	int startTime = getTime();
	while(startTime + tticks > getTime())
		asm("nop");
}

void getTimeStamp(TimeStamp* stamp)
{
	/*
	0x00 	Sekunde (BCD)
	0x02 	Minute (BCD)
	0x04 	Stunde (BCD)
	0x06 	Wochentag (BCD)
	0x07 	Tag des Monats (BCD)
	0x08 	Monat (BCD)
	0x09 	Jahr (letzten zwei Stellen) (BCD) 
	*/
	TimeStamp t;
	t.h   = cmos_read(0x04);
	t.min = cmos_read(0x02);
	t.s   = cmos_read(0x00);
	t.d   = cmos_read(0x07);
	t.m   = cmos_read(0x08);
	t.y   = cmos_read(0x09);
	*stamp = t;
}

