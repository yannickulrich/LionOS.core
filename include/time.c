/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                            Time
*/
#ifndef TIME_C
#define TIME_C
void sleepTicks(unsigned int n)
{
	int startTime = getTime();
	while(startTime+n > getTime())
	{
		asm("nop");
	}
}
long int getTime()
{
	long int li;
	asm("int $0x29" : : "a" (0x06), "b" ( &li ) );
	return li;
}
int addTimer(void *h)
{
	int id;
	asm("int $0x29" : : "a" (0x08), "b" (h), "c" ( &id ) );
	return 0;
}
void deleteTimer( int id )
{
	asm("int $0x29" : : "a" (0x09), "b" (id) );
}
#endif
