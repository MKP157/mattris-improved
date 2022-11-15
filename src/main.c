#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

#include "blockstruct.h"
#include "rembrandt.c"
//#include "locknload.h"

#define ERASEFROMBOARD block_wprintw(board, x,y,&lst,0)
#define WRITETOBOARD block_wprintw(board, x,y,&lst,1)
#define CLEARREF clear();refresh()

#define XSCALE 3
#define YSCALE 2

block lst;
int arr[20][10] = {0};

/*void blockGen() {
	int r = rand() % 7;
	p_chunk x;
	load(r);
	
	for (int i = 0; i < 4; i++) {
		x = block_newChunk(i,a,b);
		block_insert(&temp, x);
	}
}

void blockToPlay() {

}*/

void generateNoise(int n, int skip) {
	//printf("Generating noise!");
	for (int i = 19; i > 20-(n*2); i--) {
		for (int j = 0; j < 10; j++) {
			if (!skip) arr[i][j] = rand() & 1;
			
			if (arr[i][j]) {
				noise_wprintw(board,i,j);
				
				// Debug:
				mvprintw(i,j+100,"%d",arr[i][j]);
			}
		}
	}
	layeredRefresh(3);
}

void menuloop(int selection) { 
	canvas(selection + 2, COLOR_BLUE - selection*2);
	refresh();
	
	int levelSelection = 0, 
		noiseSelection = 0, 
		cX = 22, 
		cY = 18, 
		menuType = 0;
	
	char ch = 'z';
	
	while ((ch != 13) && (ch != 'x')) {
		switch(ch) {
			case 'a':
				if(!menuType && levelSelection) {
					mvprintw(17,21 + levelSelection-- * 5,"  "); 	// menuType=0 and levelSelection > 0
					mvprintw(17,21 + levelSelection * 5,"->");
				}
				if(menuType && noiseSelection) {
					mvprintw(24,21 + noiseSelection-- * 5,"  "); 	// menuType=1 and noiseSelection > 0
					mvprintw(24,21 + noiseSelection * 5,"->");
				}
			break;
			
			case 'd':
				if(!menuType && levelSelection < 8) {
					mvprintw(17,21 + levelSelection++ * 5,"  ");	// menuType=0 and levelSelection < 9
					mvprintw(17,21 + levelSelection * 5,"->");
				}
				if(menuType && noiseSelection < 8) {
					mvprintw(24,21 + noiseSelection++ * 5,"  ");	// menuType=1 and noiseSelection < 9
					mvprintw(24,21 + noiseSelection * 5,"->");
				}
			break;
			
			case 'w':
				if(menuType && selection) { menuType--; mvprintw(17,13,"-->"); mvprintw(24,13,"   "); }
			break;
			
			case 's':
				if(!menuType && selection) { menuType++; mvprintw(17,13,"   "); mvprintw(24,13,"-->"); }
			break;
			
			default: break;
		} 
		
		// For debugging:
		mvprintw(0,0,"%d",levelSelection);
		mvprintw(1,0,"%d",noiseSelection);
		mvprintw(2,0,"%d",menuType);
		mvprintw(3,0,"%d",selection);
		
		refresh();
		
		ch = getchar();
	}
	
	CLEARREF;
	
	if (ch == 'e') { titleloop(); }
	
	else { gameloop(++levelSelection, ++noiseSelection, selection); }	
}

void titleloop() {
	canvas(1,COLOR_WHITE);
	
	// Not included in title screen file.
	mvprintw(13,8,"Game Type A");
	mvprintw(15,8,"Game Type B");
	mvprintw(1,101, "Controls  --------------");
	mvprintw(3,101, "[W]         Rotate Block");
	mvprintw(4,101, "[A]  Move Block Leftward");
	mvprintw(5,101, "[S] Move Block Rightward");
	mvprintw(6,101, "[D]  Move Block Downward");
	mvprintw(8,101, "[Enter]            Pause");
	mvprintw(9,101, "[E]        Quit to Title");
	
	mvprintw(13,21, "(*)");
	refresh();
	
	int selection = 0;
	char ch = 'a';
	
	while (ch != 13) {
		switch(ch) {
			case 'w':
				mvprintw(13,21, "(*)");
				mvprintw(15,21, "   ");
				refresh();
				selection = 0;
			break;
		
			case 's':
				mvprintw(13,21, "   ");
				mvprintw(15,21, "(*)");
				refresh();
				selection = 1;
			break;
			
			default: break;
		} 
		// char debug printf("%d", ch);
		ch = getchar();
	}
	CLEARREF;
	
	menuloop(selection);
}

void sighandler(int signum) {
	
	/*if (CHECK_COLLISION(3)) { // default case for collision detection
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
	
	refresh();*/
}

void gameloop(int level, int noise, int selection) {
	//blockGen();
	//blockToPlay();
	
	gameWindowInit();
	
	int x = 0*XSCALE, y = 0*YSCALE;
	
	// Type A and Type B differentiate here. ----------------
	// Type A = 0
		canvas(4 + selection,COLOR_BLUE);
		if (!selection) {
			canvas(4 + selection,COLOR_BLUE);
			eraseBoard();
			layeredRefresh(3);
			// Unpause animation doubles as a starting animation
			gameunpause();
		}
	// Type B = 0
		else {
			canvas(4 + selection,COLOR_GREEN);
			eraseBoard();
			layeredRefresh(3);
			gameunpause();
			generateNoise(noise, 0);
		}
	// ------------------------------------------------------
	
	// Starting position
	// Coordinates work as follows: x or y = (board array pos)*(scale)
	x = 5*XSCALE;
	y = 0*YSCALE;
	WRITETOBOARD;
	signal(SIGALRM,sighandler); // Register signal handler
	ualarm((useconds_t)(1000000 / level), 0);
	
	char ch = 'e';
	
	while (ch != 'x') {
		switch(ch) {
		case 'w':
			//if (((y-1) != -1) && collision(x,y,1)) {
			if (((y-1) != -1)) {
				ERASEFROMBOARD;
				y -= 2;
				WRITETOBOARD;
			}
		break;
		
		case 'a':
			if ((x-1) != -1) {
				ERASEFROMBOARD;
				x -= 3;
				WRITETOBOARD;
			}
		break;
		
		case 's':
			if ((y+2) < 38) {
				ERASEFROMBOARD;
				y += 2;
				WRITETOBOARD;
			}
		break;
		
		case 'd':
			if ((x+3) < 27) {
				ERASEFROMBOARD;
				x += 3;
				WRITETOBOARD;
			}
		break;
		
		case 13:
			ualarm(0, 0);	// Cancel alarm
			gamepause();
			gameunpause();
			WRITETOBOARD;
			layeredRefresh(3);
		break;
		
		default: break;
		}
		
		layeredRefresh(1);
		ch = getchar();
	}
	
	
	titleloop();
}


void main() {
	printf("%d", loadBlockData(6));
	getchar();
	// Required Inits, in order: curses, Rembrandt, random number generation current block and block in queue
	initscr();
	rembrandtInit();
	block_init(&lst);
	
	// Seed random numbers based off of system time. True randomness! It'll never seed the same value twice.
	time_t t;	
	srand((unsigned) time(&t));
	// -------------------------------------------
	
	pullBlock(&lst);
	
	//////////////////// to be put in menu-nav method
	titleloop();
	////////////////////////
	
	//block_print(&lst);
	//block_printw(&lst);
	
	//clear();
	//block_destroy(&lst);
	//block_printw(&lst);
	
	endwin();
}

