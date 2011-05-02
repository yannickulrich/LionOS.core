/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                 Timer
*/

#ifndef TIME_H
#define TIME_H
#define TIMER_SIZE 16
#include "syscall.h"
#include "datatypes.h" 
void* timer_routines[TIMER_SIZE];

void installTimer();
int getTime();
void USgetTime(int *time);
int addTimer(void (*handler));
void USaddTimer(void (*handler), int *id);
void deleteTimer(int id);
void timer_handler(struct regs* r);
void pit_init(int freq);
void sleepTicks(long ticks);
void getTimeStamp(TimeStamp* stamp);

/*How long does the OS run?
Let's say 1 hour:
1h        =  3600 s
How oft calles the PIT us in 1 s?
ca. 55 Hz = 55 s^-1
Ok. How many ticks are this?
3600*55   = 198000.
So far, so great: We get per hour about 200 000 ticks, ok?
What for datatypes we have?

     Name     |   Bit   |                     Size
--------------+---------+-----------------------------------------
     char     |    8    | 2^8  - 1 = 255
    short     |   16    | 2^16 - 1 = 65 535
     int      |   32    | 2^32 - 1 = 4 294 967 295
     long     |   64    | 2^64 - 1 = 18 446 744 073 709 551 615

It seems that int should be enough...
*/

long ticks;
#include "driver/cmos.h"

#include "time.c"
#endif
