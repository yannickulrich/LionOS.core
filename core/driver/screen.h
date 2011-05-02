#ifndef DRIVER_SCREEN
#define DRIVER_SCREEN
 
#define screenwidth 80
#define screenheight 25
#define INSDEL 0
#define INS 1
#define c_counter charCounter
#define DG if(DEBUG&&!POST)kprintf
#define PS if(POST)echo
#define DEBUG_MSG DG
#define pc PS
#define dl en(__LINE__)
#include <stdarg.h>
//#include "drivers/keyboard.h"
void changeColor(char bg, char fg);
void updateCursor();
void setCursorVisibility(bool visible);
void setFix(int nfix);
void clearscreen();
void printChar(char hw);
void printCharAt(char hw, int x, int y);
void kprintf(char* args, ...);
void echo(char* args, ...);
//void DEBUG_MSG(char* args, ...);
void dbg(bool sucess);
char* en(int d);


char* video = (char*) 0xb8000;    	// Graphic Card Memory
short charCounter = 0;            	// Actual Graphic Memory Counter
char* color;


bool cursor = false;
bool supressPostMessages = false;
char writingType = INSDEL;
#include "../syscall.h"
#include "screen.c"
#endif
