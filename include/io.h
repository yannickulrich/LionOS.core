/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                            IO
*/

#include "datatypes.h"
#ifndef USIO_H
#define USIO_H
#define NULL 0

#define bool int
#define boolean bool
#define true 1
#define false 0
#define screen getScreen()
Screen getScreen();
void printChar(char c);
void pn(unsigned int i);
void clearscreen();
void cheColor(Color c);
void changeColor(int fg, int bg);
void getString( char* c);
int getInt();
void kprintf(char *args, ...);
char getChar();
void flush();

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;
typedef signed long int int32_t;
typedef unsigned long int uint32_t;
typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;
typedef unsigned int size_t;
typedef unsigned char byte;
typedef unsigned int WORD;
typedef unsigned long DWORD;


typedef struct file_t {
	int ID;
	char name[8];
	uint16_t clusterID;
	char extension[3];
	int hidden;
	int isDir;
	int readOnly;
	size_t size;
	
	struct file_t *next;
} file;

typedef struct directory_t {
	int ID;
	char *name;
	uint16_t clusterID;
	file *first;
	struct directory_t *firstSub;
	
	struct directory_t *next;
} directory;
directory *getDirById( int id, directory *dir);


directory *listDir( directory *root );
char * readFile( file *f, size_t size );
void writeFile( file* f, char *towrite );
file *getFileById( int id, directory *dir);


void addKeyListener( void (*h)(char c, int scan, bool* b) );


void setFix(int nfix);
bool getBool();

void palign( char *c, int type);
#include "io.c"
#endif
