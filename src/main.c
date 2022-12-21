#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "rembrandt.c"
#include "gameloop.c"

#define CLEARREF clear();refresh()

int menuloop(int selection) {
	canvas(selection + 2, COLOR_BLUE - selection*2);
	refresh();
	
	int levelSelection = 0, 
	noiseSelection = 0,
	menuType = 0;
	
	char ch = 'p';
	
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
	
	if (ch == 'x') return 0;
	
	++levelSelection; ++noiseSelection; 
	return gameloop(&levelSelection, &noiseSelection, &selection); 
}

int titleloop() {
	canvas(1,COLOR_WHITE);
	
	mvprintw(30,101, "=+ High Scores +========");
	
	// Not included in title screen file.
	mvprintw(1,101, "=+ Controls +===========");
	mvprintw(3,101, "[W]         Rotate Block");
	mvprintw(4,101, "[A]  Move Block Leftward");
	mvprintw(5,101, "[S] Move Block Rightward");
	mvprintw(6,101, "[D]  Move Block Downward");
	mvprintw(8,101, "[Enter]            Pause");
	mvprintw(9,101, "[X]            Quit Game");
	
	refresh();
	
	int selection = 0;
	char ch = 'p';
	
	while (ch != 13 && ch != 'x') {
		switch(ch) {
			case 'w':
				mvprintw(12,2, "\\`-._");
				mvprintw(13,2, "/.-' ");
				mvprintw(18,2, "     ");
				mvprintw(19,2, "     ");
				refresh();
				selection = 0;
			break;
		
			case 's':
				mvprintw(18,2, "\\`-._");
				mvprintw(19,2, "/.-' ");
				mvprintw(12,2, "     ");
				mvprintw(13,2, "     ");
				refresh();
				selection = 1;
			break;
			
			default: break;
		} 
		// char debug printf("%d", ch);
		ch = getchar();
	}
	CLEARREF;
	
	if (ch == 'x') return 1;
	else
		return menuloop(selection);
}

void main() {
	// Required Inits, in order: curses, Rembrandt, random number generation current block and block in queue
	initscr();
	rembrandtInit();
	curs_set(0);
	
	// Seed random numbers based off of system time. True randomness! It'll never seed the same value twice.
	time_t t;	
	srand((unsigned) time(&t));
	// -------------------------------------------
	
	//////////////////// to be put in menu-nav method
	int quit = 0;
	do {
		quit = titleloop();
	} while (!quit);
	////////////////////////
	curs_set(1);
	endwin();
}

