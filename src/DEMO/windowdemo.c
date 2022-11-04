#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

#include "../rembrandt.c"

void main() {
	initscr();
	
	//for (int i = 0; i < 30; i++)
	//	mvprintw(i,0,"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	//refresh();
	
	int h = 10, w = 10;
	
	// background -----------------------------------
	rembrandtInit();
	
	canvas(3,3);
	// ----------------------------------------------
	
	gameWindowInit();
	
	layeredRefresh(3);
	char ch = getchar();
	
	
	// !!!! ANY CHANGES TO THE BACKGROUND *NECESSITATE* RE-OVERWRITES AND REFRESHES
	// see layered refresh
	canvas(2,2);
	
	layeredRefresh(3);
	
	getchar();
	
	palette(2);
	mvwprintw(board,10,10,"PENIS");
	
	wrefresh(board);
	
	getchar();
	
	/*do {
		switch (ch) {
			case 'w'
		
	} while (ch != 'e');*/
	
	endwin();
}
