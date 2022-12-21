#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "rembrandt.c"
#include "gameloop.c"

#define CLEARREF clear();refresh()
scoreList slst;	

int main_smallestScore() { return scoreList_smallest(&slst); }

void addScore(int *s) {
	char initials[4];
	for (int i = 0; i < 4;) {
		char ch = getchar();
		move(32, 39 + i*3);
		if (ch >= 'A' && ch <= 'Z') {
			initials[i++] = ch;
			printw("%c", ch);
		}
		refresh();
	} 
	initials[4] = 0;
	
	p_score x = scoreList_newScore(initials, *s);
	scoreList_insert(&slst, x);
	//scoreList_print(&slst);
	saveScoreData(&slst);
}

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
		//mvprintw(0,0,"%d",levelSelection);
		//mvprintw(1,0,"%d",noiseSelection);
		//mvprintw(2,0,"%d",menuType);
		//mvprintw(3,0,"%d",selection);
		
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
	scoreList_wprint(&slst,21,101);
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
	
	scoreList_init(&slst);
	loadScoreData(&slst);
	//////////////////// to be put in menu-nav method
	int quit = 0;
	while (!quit) {
		quit = titleloop();
	}
	////////////////////////
	scoreList_delete(&slst);
	curs_set(1);
	endwin();
}

