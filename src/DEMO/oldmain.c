/* code by matt peterson. pls don't copy :( */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

#include "rembrandt.c"

// Global Variables /////////////////////////////////////////////////////////////////////

int ANCHOR_Y = 0, ANCHOR_X = 0, USER_SCORE = 0, NEXT_ID = 0, CURRENT_ID = 0, BLOCK_ROTATION = 0;
int GAMEBOARD[20][10] = {0};
int BLOCK_ARRAY[4][4] = {0};
int USER_LEVEL = 9;
int PAUSE = 0;

// Methods //////////////////////////////////////////////////////////////////////////////

void sighandler(int);

void DEBUG() {
	move(0, 15);
	printw("x: %d", ANCHOR_X);
	move(3, 15);
	printw("y: %d ", ANCHOR_Y);
	
	move(5, 15);
	printw("USER_SCORE:%08d", USER_SCORE);
	
	move(0,30);
	printw("block state in memory:");
	for (int i = 0; i < 4; i++) {
		move(i,30);
		for (int j = 0; j < 4; j++)
			printw("%d", BLOCK_ARRAY[i][j]);
	}
	
	
	for (int i = 0; i < 20; i++) {
		move(i, 50);
		for (int j = 0; j < 10; j++) {
			printw("%d", GAMEBOARD[i][j]);
		}
	}
}


// Generate new tetromino

void BLOCK_GENERATE() {
	NEXT_ID = rand() % 7;
	move(0,0);
	printw("%d", NEXT_ID);
	
	move(8, 15);
	printw("Next block:");
	move(9,15);
	
	switch(NEXT_ID) {
			case 0:	// T
			printw("XXX ");
			move(10,15);
			printw(" X  ");
			break;
			
			case 1:	// I
			printw("    ");
			move(10,15);
			printw("XXXX");
			break;
			
			case 2: // O
			printw(" XX ");
			move(10,15);
			printw(" XX ");
			break;
			
			case 3:  // J
			printw("X   ");
			move(10,15);
			printw("XXX ");
			break;
			
			case 4: // L
			printw("XXX ");
			move(10,15);
			printw("X   ");
			break;
			
			case 5: // S
			printw(" XX ");
			move(10,15);
			printw("XX  ");
			break;
			
			case 6: // Z
			printw(" XX ");
			move(10,15);
			printw("  XX");
			break;
			
			default: break;
	}
}


// Set tetromino in play /////////////////////////////////////////////////////////////

void BLOCK_PULL() {
	CURRENT_ID = NEXT_ID;
	BLOCK_ROTATION = 0;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			BLOCK_ARRAY[i][j] = 0;
		}
	}
	
	switch(NEXT_ID) {
			case 0:	// T
			BLOCK_ARRAY[0][0] = 1;
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[0][2] = 1;
			BLOCK_ARRAY[1][1] = 1;
			break;
			
			case 1:	// I
			BLOCK_ARRAY[0][0] = 1;
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[0][2] = 1;
			BLOCK_ARRAY[0][3] = 1;
			break;
			
			case 2: // O
			BLOCK_ARRAY[0][0] = 1;
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[1][0] = 1;
			BLOCK_ARRAY[1][1] = 1;
			break;
			
			case 3:  // J
			BLOCK_ARRAY[0][0] = 1;
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[0][2] = 1;
			BLOCK_ARRAY[1][2] = 1;
			break;
			
			case 4: // L
			BLOCK_ARRAY[0][0] = 1;
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[0][2] = 1;
			BLOCK_ARRAY[1][0] = 1;
			break;
			
			case 5: // S
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[0][2] = 1;
			BLOCK_ARRAY[1][0] = 1;
			BLOCK_ARRAY[1][1] = 1;
			break;
			
			case 6: // Z
			BLOCK_ARRAY[0][0] = 1;
			BLOCK_ARRAY[0][1] = 1;
			BLOCK_ARRAY[1][1] = 1;
			BLOCK_ARRAY[1][2] = 1;
			break;
			
			default: break;
	}
	
	BLOCK_GENERATE();
}


// draw active block. parameter x: 0=erase, 1=draw

void BLOCK_DRAW(int x) {
	move(ANCHOR_Y, ANCHOR_X);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (BLOCK_ARRAY[i][j] == 1) {
				move(ANCHOR_Y+i,ANCHOR_X+j);
				if (x)
					printw("#");
				else
					printw(" ");
			}
		}
	}
	move(ANCHOR_Y, ANCHOR_X);
}


// rotate block both in theory and on game GAMEBOARD[
void BLOCK_ROTATE() {

	if (CURRENT_ID != 2) {
	
	    	BLOCK_ROTATION++;
    		
    		// matrix inversion algorithm
		for (int x = 0; x < (4 / 2); x++) {
			for (int y = x; y < (4 - x - 1); y++) {
				int temp = BLOCK_ARRAY[x][y];
				// Move values from right to top
				BLOCK_ARRAY[x][y] = BLOCK_ARRAY[y][4 - 1 - x];
				// Move values from bottom to right
				BLOCK_ARRAY[y][4 - 1 - x] = BLOCK_ARRAY[4 - 1 - x][4 - 1 - y];
				// Move values from left to bottom
				BLOCK_ARRAY[4 - 1 - x][4 - 1 - y] = BLOCK_ARRAY[4 - 1 - y][x];
				// Assign temp to left
				BLOCK_ARRAY[4 - 1 - y][x] = temp;
        		}
    		}
		
    	// these next lines correct the positioning of the matrix depending on the block,
    	// so that all blocks and their orientations are anchored to the top-left corner
    	// of the active-block matrix
    		if (CURRENT_ID != 1) {
    			for (int i = (1 + (BLOCK_ROTATION-1) % 2); i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					BLOCK_ARRAY[i-( 1 + ((BLOCK_ROTATION-1) % 2))][j] = BLOCK_ARRAY[i][j];
					BLOCK_ARRAY[i][j] = 0;
				}
			}
			if (BLOCK_ROTATION == 4) BLOCK_ROTATION = 0;
		}
		
		// below switches the I's second state back to first.
		else {
			if (BLOCK_ROTATION == 2) {
				for (int j = 0; j < 4; j++) {
					BLOCK_ARRAY[0][j] = BLOCK_ARRAY[3][j];
					BLOCK_ARRAY[3][j] = 0;
				}
				BLOCK_ROTATION = 0;
			}
		}
    	}
}

// Check collisions. Return 0 if no, 1 if yes

int CHECK_COLLISION(int choice) {	
	int result = 1;
	int modX, modY, n;
	
	switch(choice) {
		case 0:	// 0 = left
		modX = -1; modY = 0;
		break;
		
		case 1: //1 = right
		modX = 1; modY = 0;
		break;
		
		case 2: // 2 = rotation
		modX = 0; modY = 0;
		
		default: //3 = down
		modX = 0; modY = 1;
		break;
	}
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (BLOCK_ARRAY[i][j]) {
				if ((ANCHOR_X+j+modX != 10) && (ANCHOR_X+j+modX != -1) && (ANCHOR_Y+i+modY != 20)) {
					n = BLOCK_ARRAY[i][j] + GAMEBOARD[ANCHOR_Y+i+modY][ANCHOR_X+j+modX];
					result = result && (n != 2);
				}
				
				else result = 0;
			}
			if (!result) break;
		}
		if (!result) break;
	}
	
	return result;
}



// Check for full line /////////////////////////////////////////////////////////////

void CHECKLINE(int line) {
	int compare = 1;
	
	// if row full, compare will exit as 1.
	for (int j = 0; j < 10; j++) {
		compare = compare && GAMEBOARD[line][j];
	}
	
	if (compare) { //if row full, compare = 1 or TRUE
		
		PAUSE = 1;
		
		ualarm(0,1000000);
		mvprintw(line,0,"----------"); refresh(); usleep(62500);
		mvprintw(line,0,"//////////"); refresh(); usleep(62500);
		mvprintw(line,0,"||||||||||"); refresh(); usleep(62500);
		mvprintw(line,0,"\\\\\\\\\\"); refresh(); usleep(62500);
		
		for (int i = line; i > 0; i--) {
		
			move(i,0);
			for (int j = 0; j < 10; j++) {
				
				GAMEBOARD[i][j] = GAMEBOARD[i-1][j];
				
				if (GAMEBOARD[i][j])
					printw("#");
				else
					printw(" ");
			}
		}
		
		USER_SCORE += 100;
		
		if (!(USER_SCORE % 1000)) USER_LEVEL--;
		
		PAUSE = 0;
	}
}

// Write block to game GAMEBOARD[

void BLOCK_WRITE() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (BLOCK_ARRAY[i][j]) {
				GAMEBOARD[ANCHOR_Y+i][ANCHOR_X+j] = 1;
				CHECKLINE(ANCHOR_Y+i);
			}
		}
	}
}

// Game loop /////////////////////////////////////////////////////////////

void GAME_LOOP() {

	// draw blank GAMEBOARD[
	for (int i = 0; i < 40; i++){ // 2 wide
		for (int j = 0; j < 30; j++){ // 3 wide
			addch(ACS_CKBOARD);
			move(i,j);
		}
	}

	refresh();
	getchar();

	for (int i = 0; i < 20; i++){
		move(i,10);
		printw("|");
	}
	
	
	BLOCK_GENERATE();
	BLOCK_PULL();
	
	ANCHOR_Y = 0;
	ANCHOR_X = 4;
	BLOCK_DRAW(1);
	
	int ch = 'p';
	
	refresh();
	
	signal(SIGALRM,sighandler); // Register signal handler
	ualarm((useconds_t)(USER_LEVEL * 100000), 0);
			
	while (ch != 'e') {
		if (!PAUSE) {
		switch(ch) {
			case 'w':	// rotate
			BLOCK_DRAW(0);
			BLOCK_ROTATE();
			if (!CHECK_COLLISION(2)) {
				BLOCK_ROTATE(); BLOCK_ROTATE(); BLOCK_ROTATE();
			}
			BLOCK_DRAW(1);
			break;
			
			case 'a':	// check left:
			if (CHECK_COLLISION(0)) {
				BLOCK_DRAW(0);
				ANCHOR_X = ANCHOR_X - 1;
				BLOCK_DRAW(1);
			}
			break;
			
			case 'd':	// check right:
			if (CHECK_COLLISION(1)) {
				BLOCK_DRAW(0);
				ANCHOR_X = ANCHOR_X + 1;
				BLOCK_DRAW(1);
			}
			break;
			
			case 's':
				sighandler(SIGALRM);
			break;
			
			// USER_LEVELs are broken. implementing later
			//case 'l':
			//USER_LEVEL--;
			//break;
			
			default: break;
		}
		}
		
		DEBUG();
		
		refresh();
		ch = getchar();
	}
}

// Signal handler (move down) /////////////////////////////////////////////////////////////

void sighandler(int signum) {
	
	if (CHECK_COLLISION(3)) { // default case for collision detection
		BLOCK_DRAW(0);
		ANCHOR_Y = ANCHOR_Y + 1;
		BLOCK_DRAW(1);
		
		ualarm((useconds_t)(USER_LEVEL * 100000), 0);
		
		DEBUG();
		move(ANCHOR_Y, ANCHOR_X);
	}
	else {
		BLOCK_WRITE();
		BLOCK_PULL();
		
		ANCHOR_X = 4;
		ANCHOR_Y = 0;
		
		BLOCK_DRAW(1);
		refresh();
		
		DEBUG();
		sighandler(SIGALRM);
	}
	
	refresh();
		
}

// Main /////////////////////////////////////////////////////////////

int main(){
	time_t t;
	srand((unsigned) time(&t));	// Use current time to seed random number generation
	
	initscr();			// Begin curses
	GAME_LOOP();			// active game code
	endwin();			// End curses mode

	return 0;
}

// Planned GUI:
// window is 36x21 chars
//.....____________________...........
//....|....................|..........
//....|....................|..USER_SCORE:..
//....|....................|.{XXXXXX}.
//....|....................|..........
//....|....................|..........
//....|....................|..Next:...
//....|....................|...XXXX...
//....|....................|...XXXX...
//....|....................|..........
//....|....................|..........
//....|....................|..........
//....|....................|..........
//....|....................|..........
//....|....................|..........
//....|....................|..........
//....|....................|..........
//....|....................|..mattress
//....|....................|.by matt p
//....|....................|..........
//....|....................|..........
//....|....................|..........