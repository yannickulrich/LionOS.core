/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                           Stdlib
*/
#ifndef STDLIB_H
#define STDLIB_H
void initTask(void *handler, int priotity);
void idle();
void free(void *p);
void *malloc(int size);
void installShortCut(char c, void *h);
void exit(int state);
int runAPP( file *f );

void reboot();

int rand();
int setrand(int max);

#include <time.h>

#include <stdlib.c>

#endif
