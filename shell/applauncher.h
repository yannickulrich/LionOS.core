/**
     ####  #   # ##### #     #     
    #      #   # #     #     #
     ####  ##### ####  #     #
         # #   # #     #     #
     ####  #   # ##### ##### #####
             AppLauncher
 * This module includes:
 *  - launchApp()
 *  - initLauncher(void (*h)(int id))
 * Include-Codes: #_APPL___H ; #_APPL___C
 */
#ifndef _APPL___H
#define _APPL___H
#define EXECUTABLE 0
#define FOLDER 1
typedef struct {
	char name[8];
	int id;
	int extype;
} App;
void initLauncher();
void launchApp();
#include "applauncher.c"
#endif
