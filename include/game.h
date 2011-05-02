/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                         Game
*/
#include "datatypes.h"
#include "game.c"
#ifndef UM_Game
#define UM_Game
typedef struct {
	int xpos;
	int ypos;
	char character;
	Color color;
} GameObject;
typedef struct {
	Color bg;
} GameScreen;

//void paintGame(GameScreen screen, GameObject objects[], int onum);
#endif
