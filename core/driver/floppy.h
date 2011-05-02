/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   Floppy
*/

#ifndef FLOPPY_H
#define FLOPPY_H
#define NUM_OF_FDD     1
#define SECTORS_PER_TRACK 18

const int sysbuffer = 0x1000;
enum FDCREG
{
        STATUS_REGISTER_A                = 0x3F0, // read-only
        STATUS_REGISTER_B                = 0x3F1, // read-only
        DOR                              = 0x3F2,
        TAPE_DRIVE_REGISTER              = 0x3F3,
        MSR                              = 0x3F4, // read-only
        DATARATE_SELECT_REGISTER         = 0x3F4, // write-only
        DATA_FIFO                        = 0x3F5,
        DIGITAL_INPUT_REGISTER           = 0x3F7, // read-only
        CCR                              = 0x3F7  // write-only
};


enum FDCCMD
{
        READ_TRACK =                 0x2,       // generates IRQ6
        SPECIFY =                    0x3,      // * set drive parameters
        SENSE_DRIVE_STATUS =         0x4,
        WRITE_DATA =                 0x5,      // * write to the disk
        READ_DATA =                  0x6,      // * read from the disk
        RECALIBRATE =                0x7,      // * seek to cylinder 0
        SENSE_INTERRUPT =            0x8,      // * ack IRQ6, get status of last command
        WRITE_DELETED_DATA =         0x9,
        READ_ID =                    10,        // generates IRQ6
        READ_DELETED_DATA =          12,
        FORMAT_TRACK =               13,     // *
        SEEK =                       0xF,     // * seek both heads to cylinder X
        VERSION =                    0x10,      // * used during initialization, once
        SCAN_EQUAL =                 17,
        PERPENDICULAR_MODE =         18,        // * used during initialization, once, maybe
        CONFIGURE =                  0x13,     // * set controller parameters
        LOCK =                       0x96,     // * protect controller params from a reset
        UNLOCK =                     0x14,
        VERIFY =                     22,
        SCAN_LOW_OR_EQUAL =          25,
        SCAN_HIGH_OR_EQUAL =         29
};


void resetFDD();
void initFDD();
void configure(int drive_polling_mode, int FIFO, int threshold, int implied_seek, int precompensation);
int writeSector(int sectorLBA);
int readSector(int sectorLBA);

typedef struct {
	int C; int H; int S;
} CHS;

typedef struct {
	CHS position;
	int motor;
		
} FDD;

typedef struct {
	int lock;
	int drive_polling_mode; int FIFO; int threshold; int implied_seek; int precompensation;
	int acDrive;
	int irq;
	FDD drives[NUM_OF_FDD];
} fdcState;

fdcState fdc;


#include "floppy.c"
#include "../fat.h"
#endif
