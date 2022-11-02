#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

#define SCALEY 2
#define SCALEX 3

void rMove(int boardY, int boardX){
    move(boardY*SCALEY, boardX*SCALEX);
}

void rDraw(int OPERATION) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (global.BLOCK_ARRAY[i][j] == 1) {
				move(ANCHOR_Y+i,ANCHOR_X+j);
				if (x)
					printw("#");
				else
					printw(" ");
			}
		}
	}
}
