/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                           Stdlib
*/
#ifndef STDLIB_C
#define STDLIB_C
#include <time.c>
static int rndSeek = 0;

int rand()
{
	int rnd = ( getTime() + rndSeek) % 10000;
	rndSeek += rnd;
	return rnd;
}

int setrand(int max)
{
	int rnd = ( getTime() + rndSeek) % max;
	rndSeek += rnd;
	return rnd;
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
    char* temp = (char*)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

void initTask(void *handler, int priotity)
{
	asm("int $0x29" : : "a" (0x02), "b" (handler), "c" (priotity));
}
void idle()
{
	asm("int $0x29" : : "a" (0x03));
}
void exit(int state)
{
	asm("int $0x29" : : "a" (0x01));
	while(1);
}
void free(void *p)
{
	asm("int $0x29" : : "a" (0x05), "b" (p));
}
#include "io.c"
int runAPP( file *f )
{
	asm("int $0x29" : : "a" (0x01), "b" (f));
	return 0x00;
}
void *malloc(int size)
{
	void *p;
	asm("int $0x29" : : "a" (0x04), "b" (&p) );
	return p;
}
void installShortCut(char c, void *h)
{
	//asm("int $0x29" : : "a" (11), "b" (c), "c" (h) );
	asm("int $0x29" : : "a" (0x1f));
}

void reboot()
{
	asm("int $0x29" : : "a" (0x00));
}
#endif
