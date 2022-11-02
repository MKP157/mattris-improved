/* code by matt peterson. pls don't copy :( */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

// Global Variables /////////////////////////////////////////////////////////////////////

int ULPY = 0, ULPX = 0, score = 0, nextBlock = 0, currentBlock = 0, blockState = 0;
int board[20][10] = {0};
int block[4][4] = {0};
int level = 9;
int pauseIn = 0;

// Methods //////////////////////////////////////////////////////////////////////////////

void sighandler(int);

void DEBUG() {
	move(0, 15);
	printw("x: %d", ULPX);
	move(3, 15);
	printw("y: %d ", ULPY);
	
	move(5, 15);
	printw("Score:%08d", score);
	
	move(0,30);
	printw("block state in memory:");
	for (int i = 0; i < 4; i++) {
		move(i,30);
		for (int j = 0; j < 4; j++)
			printw("%d", block[i][j]);
	}
	
	
	for (int i = 0; i < 20; i++) {
		move(i, 50);
		for (int j = 0; j < 10; j++) {
			printw("%d", board[i][j]);
		}
	}
}


// Generate new tetromino

void generateBlock() {
	nextBlock = rand() % 7;
	move(0,0);
	printw("%d", nextBlock);
	
	move(8, 15);
	printw("Next block:");
	move(9,15);
	
	switch(nextBlock) {
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

void activeBlock() {
	currentBlock = nextBlock;
	blockState = 0;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = 0;
		}
	}
	
	switch(nextBlock) {
			case 0:	// T
			block[0][0] = 1;
			block[0][1] = 1;
			block[0][2] = 1;
			block[1][1] = 1;
			break;
			
			case 1:	// I
			block[0][0] = 1;
			block[0][1] = 1;
			block[0][2] = 1;
			block[0][3] = 1;
			break;
			
			case 2: // O
			block[0][0] = 1;
			block[0][1] = 1;
			block[1][0] = 1;
			block[1][1] = 1;
			break;
			
			case 3:  // J
			block[0][0] = 1;
			block[0][1] = 1;
			block[0][2] = 1;
			block[1][2] = 1;
			break;
			
			case 4: // L
			block[0][0] = 1;
			block[0][1] = 1;
			block[0][2] = 1;
			block[1][0] = 1;
			break;
			
			case 5: // S
			block[0][1] = 1;
			block[0][2] = 1;
			block[1][0] = 1;
			block[1][1] = 1;
			break;
			
			case 6: // Z
			block[0][0] = 1;
			block[0][1] = 1;
			block[1][1] = 1;
			block[1][2] = 1;
			break;
			
			default: break;
	}
	
	generateBlock();
}


// draw active block. parameter x: 0=erase, 1=draw

void drawBlock(int x) {
	move(ULPY, ULPX);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[i][j] == 1) {
				move(ULPY+i,ULPX+j);
				if (x)
					printw("#");
				else
					printw(" ");
			}
		}
	}
	move(ULPY, ULPX);
}


// rotate block both in theory and on game board
void rotateBlock() {

	if (currentBlock != 2) {
	
	    	blockState++;
    		
    		// matrix inversion algorithm
		for (int x = 0; x < (4 / 2); x++) {
			for (int y = x; y < (4 - x - 1); y++) {
				int temp = block[x][y];
				// Move values from right to top
				block[x][y] = block[y][4 - 1 - x];
				// Move values from bottom to right
				block[y][4 - 1 - x] = block[4 - 1 - x][4 - 1 - y];
				// Move values from left to bottom
				block[4 - 1 - x][4 - 1 - y] = block[4 - 1 - y][x];
				// Assign temp to left
				block[4 - 1 - y][x] = temp;
        		}
    		}
		
    	// these next lines correct the positioning of the matrix depending on the block,
    	// so that all blocks and their orientations are anchored to the top-left corner
    	// of the active-block matrix
    		if (currentBlock != 1) {
    			for (int i = (1 + (blockState-1) % 2); i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i-( 1 + ((blockState-1) % 2))][j] = block[i][j];
					block[i][j] = 0;
				}
			}
			if (blockState == 4) blockState = 0;
		}
		
		// below switches the I's second state back to first.
		else {
			if (blockState == 2) {
				for (int j = 0; j < 4; j++) {
					block[0][j] = block[3][j];
					block[3][j] = 0;
				}
				blockState = 0;
			}
		}
    	}
}

// Check collisions. Return 0 if no, 1 if yes

int checkCollision(int choice) {	
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
			if (block[i][j]) {
				if ((ULPX+j+modX != 10) && (ULPX+j+modX != -1) && (ULPY+i+modY != 20)) {
					n = block[i][j] + board[ULPY+i+modY][ULPX+j+modX];
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

void checkLine(int line) {
	int compare = 1;
	
	// if row full, compare will exit as 1.
	for (int j = 0; j < 10; j++) {
		compare = compare && board[line][j];
	}
	
	if (compare) { //if row full, compare = 1 or TRUE
		
		pauseIn = 1;
		
		ualarm(0,1000000);
		mvprintw(line,0,"----------"); refresh(); usleep(62500);
		mvprintw(line,0,"//////////"); refresh(); usleep(62500);
		mvprintw(line,0,"||||||||||"); refresh(); usleep(62500);
		mvprintw(line,0,"\\\\\\\\\\"); refresh(); usleep(62500);
		
		for (int i = line; i > 0; i--) {
		
			move(i,0);
			for (int j = 0; j < 10; j++) {
				
				board[i][j] = board[i-1][j];
				
				if (board[i][j])
					printw("#");
				else
					printw(" ");
			}
		}
		
		score += 100;
		
		if (!(score % 1000)) level--;
		
		pauseIn = 0;
	}
}

// Write block to game board

void writeBlock() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				board[ULPY+i][ULPX+j] = 1;
				checkLine(ULPY+i);
			}
		}
	}
}

// Game loop /////////////////////////////////////////////////////////////

void gameloop() {

	// draw blank board
	for (int i = 0; i < 20; i++){
		move(i,10);
		printw("|");
	}
	
	
	generateBlock();
	activeBlock();
	
	ULPY = 0;
	ULPX = 4;
	drawBlock(1);
	
	int ch = 'p';
	
	refresh();
	
	signal(SIGALRM,sighandler); // Register signal handler
	ualarm((useconds_t)(level * 100000), 0);
			
	while (ch != 'e') {
		if (!pauseIn) {
		switch(ch) {
			case 'w':	// rotate
			drawBlock(0);
			rotateBlock();
			if (!checkCollision(2)) {
				rotateBlock(); rotateBlock(); rotateBlock();
			}
			drawBlock(1);
			break;
			
			case 'a':	// check left:
			if (checkCollision(0)) {
				drawBlock(0);
				ULPX = ULPX - 1;
				drawBlock(1);
			}
			break;
			
			case 'd':	// check right:
			if (checkCollision(1)) {
				drawBlock(0);
				ULPX = ULPX + 1;
				drawBlock(1);
			}
			break;
			
			case 's':
				sighandler(SIGALRM);
			break;
			
			// levels are broken. implementing later
			//case 'l':
			//level--;
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

// Main /////////////////////////////////////////////////////////////

int main(){
	time_t t;
	srand((unsigned) time(&t));	// Use current time to seed random number generation
	
	initscr();			// Begin curses
	gameloop();			// active game code
	endwin();			// End curses mode

	return 0;
}

// Planned GUI:
// window is 36x21 chars
//.....____________________...........
//....|....................|..........
//....|....................|..Score:..
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