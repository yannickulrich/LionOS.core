/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                           Dialog
 * @author AE, AG
 */
#ifndef DIALOG
#define DIALOG 1
#define OK_CANCEL_OPTION 0
#define YES_NO_OPTION 1
#define YES 0
#define NO 1
#define OK YES
#define CANCEL NO
#include "io.h"
#include "strings.h"
int showDialog(char text[], char title[], int optionType);
#include "dialog.c"
#endif
