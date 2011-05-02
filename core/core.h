/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   Core
*/
//#define PS pc
#ifndef COREH
#define COREH 1
void USgetKernel(char* start);
void setAskKeycodes(bool value);
void initScreen();
void initKeycodes();
void printLogo();
#include "core.c"
#endif
