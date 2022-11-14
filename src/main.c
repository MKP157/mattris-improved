#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "blockstruct.h"
#include "rembrandt.c"
//#include "physics.h"
block lst;

void main() {
	initscr();
	// draw blank board
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			mvprintw((i)*2+1,(j)*3+2,".");
		}
	}
	
	block_init(&lst);
	
	p_chunk x = block_newChunk(1,0,0);
	block_insert(&lst, x);
	
	x = block_newChunk(2,1,0);
	block_insert(&lst, x);
	
	x = block_newChunk(3,0,1);
	block_insert(&lst, x);
	
	x = block_newChunk(4,1,1);
	block_insert(&lst, x);
	
	////////////////////
	physloop();
	////////////////////////
	
	//block_print(&lst);
	//block_printw(&lst);
	
	//clear();
	//block_destroy(&lst);
	//block_printw(&lst);
	
	endwin();
}

void physloop() {
	
	int x = 0, y = 0;
	
	block_printw(x,y,&lst,1);
	
	char ch = 'e';
	
	while (ch) {
		switch(ch) {
		case 'w':
			if ((y-1) != -1) {
				block_printw(x,y,&lst,0);
				y -= 2;
				block_printw(x,y,&lst,1);
			}
		break;
		
		case 'a':
			if ((x-1) != -1) {
				block_printw(x,y,&lst,0);
				x -= 3;
				block_printw(x,y,&lst,1);
			}
		break;
		
		case 's':
			if ((y+2) < 38) {
				block_printw(x,y,&lst,0);
				y += 2;
				block_printw(x,y,&lst,1);
			}
		break;
		
		case 'd':
			if ((x+3) < 27) {
				block_printw(x,y,&lst,0);
				x += 3;
				block_printw(x,y,&lst,1);
			}
		break;
		
		default: break;
		}
		
		refresh(); ch = getchar();
	}
}
