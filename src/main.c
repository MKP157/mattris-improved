
// "mattris", a Tetris clone by Matthew Kenneth Peterson
// of the University of New Brunswick Saint John
// (student ID 3719754).
// ------------------------------------------------------
// 
// NAMING CONVENTIONS:
// defines/functions:   capslock underscore
// variables:           camel case
// ------------------------------------------------------


// C LIBRARY INCLUDES -----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
// ------------------------------------------------------


// CUSTOM LIBRARY INCLUDES ------------------------------
// #include "./lib/.h"
#include "./lib/load.h"
#include "./lib/collision.h"
#include "./lib/blocks.h"
#include "./lib/scenes.h"
// ------------------------------------------------------


// MACROS -----------------------------------------------
#define ANCHOR_RETURN move(A_Y,A_X)
// ------------------------------------------------------


// GLOBALS ----------------------------------------------
int ULPY = 0, ULPX = 0, score = 0, nextBlock = 0, currentBlock = 0, blockState = 0;
int board[20][10] = {0};
int block[4][4] = {0};
int level = 9;
int pauseIn = 0;
// ------------------------------------------------------


// Main -------------------------------------------------
int main() {
    
    
    /*
    time_t t;
	srand((unsigned) time(&t));	
        // Use current time to seed random number generation.
	
	initscr();			// Begin curses
	gameloop();			// Begin the active game code
	endwin();			// End curses mode after game loop exits
    */

	return 0;
}