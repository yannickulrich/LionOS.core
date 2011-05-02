/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                       Datatypes
*/

#ifndef Datatype
#define Datatype
typedef struct {
	int width;
	int height;
} Dimension;
typedef struct {
	int width;
	int height;
	int fix;
	int center;
	int right;
	int left;
} Screen;
typedef struct {
	unsigned int x;
	unsigned int y;
} Point;
typedef struct {
	unsigned short fg;
	unsigned short bg;
} Color;
typedef struct {
	char array[32];
	char value;
} MapEntry;
typedef struct {
	int length;
	MapEntry entry[];
} Map;
Color toColor(unsigned short col);
Dimension newDimension(int width, int height);
#include "datatypes.c"
#endif
