/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                    FAT
*/

#ifndef FAT_H
#define FAT_H
#define ROOT_SEKTOR 19
#define LSDIR_BUFFER 224*0x20
#define FAT_DATA_OFFSET (uint16_t)*((char*)

FATENTRY fat1[0x180];
char fatReadBuffer[512];

directory *listDir( directory *root );
int readFile( file *f , size_t size, char *des);
file *getFileById( int id, directory *dir);
void runFile(file *f, size_t size);

void USlistDir(directory *root, directory **result);
void USreadFile(file *f, char **result, size_t size);
void USrunFile(file *name, int *pid);



#include "fat.c"
#endif
