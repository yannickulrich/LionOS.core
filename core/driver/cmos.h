/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                    CMOS
*/
#ifndef Core_CMOS
#define Core_CMOS
#define CMOS_PORT_ADDRESS 0x70
#define CMOS_PORT_DATA    0x71
#define readCMOS cmos_read
#define writeCMOS cmos_write
unsigned char cmos_read(unsigned char offset);
void cmos_write(unsigned char offset, unsigned char val);
#include "cmos.c"
#endif
