/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                            Time
*/
#ifndef TIME_H
#define TIME_H
void sleepTicks(unsigned int n);
long int getTime();
int addTimer( void *h );
void deleteTimer( int id );
#include "time.c"
#endif
