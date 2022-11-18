#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

#include "blockstruct.h"
#include "rembrandt.c"
//#include "locknload.h"

#define ERASEFROMBOARD block_wprintw(board,XSCALE*x,YSCALE*y,&lst,0)
#define WRITETOBOARD block_wprintw(board,XSCALE*x,YSCALE*y,&lst,1)
#define CLEARREF clear();refresh()

#define XSCALE 3
#define YSCALE 2

block lst;
int arr[20][10] = {0};
int x = 0, y = 0;

void menuloop(int selection) { 
	canvas(selection + 2, COLOR_BLUE - selection*2);
	refresh();
	
	int levelSelection = 0, 
		noiseSelection = 0, 
		cX = 22, 
		cY = 18, 
		menuType = 0;
	
	char ch = 'A';
	
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


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /$$$$$$$  /$$   /$$ /$$     /$$ /$$$$$$  /$$$$$$  /$$$$$$   /$$$$$$ //////////////////////////////////////////////////////////////////
| $$__  $$| $$  | $$|  $$   /$$//$$__  $$|_  $$_/ /$$__  $$ /$$__  $$//////////////////////////////////////////////////////////////////
| $$  \ $$| $$  | $$ \  $$ /$$/| $$  \__/  | $$  | $$  \__/| $$  \__///////////////////////////////////////////////////////////////////
| $$$$$$$/| $$$$$$$$  \  $$$$/ |  $$$$$$   | $$  | $$      |  $$$$$$ //////////////////////////////////////////////////////////////////
| $$____/ | $$__  $$   \  $$/   \____  $$  | $$  | $$       \____  $$//////////////////////////////////////////////////////////////////
| $$      | $$  | $$    | $$    /$$  \ $$  | $$  | $$    $$ /$$  \ $$//////////////////////////////////////////////////////////////////
| $$      | $$  | $$    | $$   |  $$$$$$/ /$$$$$$|  $$$$$$/|  $$$$$$///////////////////////////////////////////////////////////////////
|__/      |__/  |__/    |__/    \______/ |______/ \______/  \______/ //////////////////////////////////////////////////////////////////
									*/////////////////////////////////////////////////////////////


int collision(char dir, p_block plst, int y, int x) {
	int modX = 0, modY = 0, newX, newY, n;
	
	switch(dir) {
		case 'a':  --modX;  break;
		
		case 'd':  ++modX;  break;
		
		case 's':  ++modY;  break;
		
		default: break;
	}
	
	p_chunk z = plst->head;
	while(z) {
		newX = x + modX + z->Rx;
		newY = y + modY + z->Ry;
		
		if (arr[newY][newX]) return 0;
		if (newX < 0 || newX > 9) return 0;
		if (newY < 0 || newY > 19) return 0;
		z = z->next;
	}
	return 1;
}

int rotation(p_block plst, int y, int x) {
	p_chunk z = plst->head;
	int tempY, tempX, newY, newX, store[8], i = 0;
	
	if (z->k == 3) return 0;
		// [!] O blocks don't need rotation!
		
	while(z) {
		// Clockwise 90deg: (x,y) -> (y,-x), or (y,x) -> (-x,y)
		
		// Center Rx=1,Ry=1 as origin:
		tempY = z->Ry - 1;
		tempX = z->Rx - 1;
		
		newY = y + (-1)*tempX + 1;
		newX = x + tempY + 1;
		
		if (arr[newY][newX]) return 0;
		if ((newX) < 0 || (newX) > 9) return 0;
		if ((newY) < 0 || (newY) > 19) return 0;
		
		// We save the new rotation coordinates for later, because if rotation is found to collide
		// somewhere down the line, we'll have edited the first coordinates and not the rest!
		store[i++] = newY - y;	
		store[i++] = newX - x;
		
		z = z->next;
	}
	
	z = plst->head; i = 0;
	while(z) {
		z->Ry = store[i++];
		z->Rx = store[i++];
		z = z->next;
	}
	
	return 1;
}

void writeBlock(p_block plst) {
	p_chunk z = plst->head;
	while(z) {
		arr[y + z->Ry][x + z->Rx]++;
		z = z->next;
	}
}

// Originally intended for generating board noise for type B games,
// however this function also doubles as a board-redraw for the pause
// menu. That's that the "skip" boolean condition is for; it skips
// randomly populating the board for when all that's needed is a skip.

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


void sighandler(int signum) {
	if (collision('s', &lst, y, x)) {
		ERASEFROMBOARD;
		y++;
		WRITETOBOARD;
		layeredRefresh(1);
		
		ualarm((useconds_t)(999999), 0);
	}
	
	else {
		x = 3;
		y = 0;
		
		writeBlock(&lst);
		
		pullBlock(&lst);
		layeredRefresh(1);
		ualarm((useconds_t)(999999), 0);
	}
}

void gameloop(int level, int noise, int selection) {
	//blockGen();
	//blockToPlay();
	
	gameWindowInit();
	
	
	x = 0, y = 0;
	
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
	x = 3;
	y = 0;
	WRITETOBOARD;
	signal(SIGALRM,sighandler); // Register signal handler
	ualarm((useconds_t)(999999), 0);
	
	char ch = 'e';
	
	while (ch != 'x') {
		switch(ch) {
		
		case 'w':
			ERASEFROMBOARD;
			if (rotation(&lst,y,x));
			WRITETOBOARD;
		break;
		
		case 'a':
			if (collision(ch, &lst, y, x)) {
				ERASEFROMBOARD;
				x--;
				WRITETOBOARD;
			}
		break;
		
		case 's':
			sighandler(SIGALRM);
			/*
			if (collision(ch, &lst, y, x)) {
				ERASEFROMBOARD;
				y++;
				WRITETOBOARD;
			}
			*/
		break;
		
		case 'd':
			if (collision(ch, &lst, y, x)) {
				ERASEFROMBOARD;
				x++;
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
		
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				mvprintw(i,j+100,"%d",arr[i][j]);
			}
		}
		
		mvprintw(50,100,"y = %d",y);
		mvprintw(51,100,"x = %d",x);
		
		layeredRefresh(3);
		ch = getchar();
	}
	
	
	titleloop();
}


void main() {
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

