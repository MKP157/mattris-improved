#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "bsOLDENG.h"
//#include "physics.h"
block lst;

void main() {
	initscr();
	
	
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
	
	int x = 5, y = 0;
	
	block_printw(x,y,&lst,1);
	
	char ch = 'e';
	
	while (ch) {
		switch(ch) {
		case 'w':
		if ((y-1) != -1) {
			block_printw(x,y,&lst,0);
			block_printw(x,--y,&lst,1);
		}
		break;
		case 'a':
		if ((x-1) != -1) {
			block_printw(x,y,&lst,0);
			block_printw(--x,y,&lst,1);
		}
		break;
		case 's':
			block_printw(x,y,&lst,0);
			block_printw(x,++y,&lst,1);
		break;
		case 'd':
			block_printw(x,y,&lst,0);
			block_printw(++x,y,&lst,1);
		break;
		
		default: break;
		}
		
		refresh(); ch = getchar();
	}
}
