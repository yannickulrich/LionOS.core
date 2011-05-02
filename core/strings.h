/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                           Strings
*/
#ifndef STRINGS_H
#define STRING_H
#include "datatypes.h"
  
int cpystr( char dest[], char src[] ); //Copy (replace) stringTwo in stringOne. Meaning stringTwo is the source
//int cmpstr( char stringOne[], char stringTwo[] ); //Compares stringOne with stringTwo.
void appendstr( char stringOne[], char stringTwo[] ); //Append stringTwo to stringOne
bool startsWith(char one[], char start[]);
int indexOf(char src[], char dest[]);
int lastIndexOf(char src[], char dest[]);
bool contains(char src[], char search[]);
void substringToEnd(char src[], int startpos);
void substring(char src[], int startpos, int stoppos);
int strlen(char value[]);
#include "strings.c"
#endif
