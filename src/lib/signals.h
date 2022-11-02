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
#include "load.h"
#include "collision.h"
#include "blocks.h"
#include "scenes.h"
// ------------------------------------------------------




void sighandler(int signum) {
	
	if (checkCollision(3)) { // default case for collision detection
		drawBlock(0);
		ULPY = ULPY + 1;
		drawBlock(1);
		
		ualarm((useconds_t)(level * 100000), 0);
		
		DEBUG();
		move(ULPY, ULPX);
	}
	else {
		writeBlock();
		activeBlock();
		
		ULPX = 4;
		ULPY = 0;
		
		drawBlock(1);
		refresh();
		
		DEBUG();
		sighandler(SIGALRM);
	}
	
	refresh();
		
}