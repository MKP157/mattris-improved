#include <stdio.h>
#include <curses.h>

void main {
	initscr();
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			mvprintw(i,j,'.');
		}
	}
	
	
	
	
	endwin();
}
