#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "../blockstruct.h"
//#include "physics.h"

#define REFRESHWAIT refresh(); getchar()

void main() {
	initscr();
	
	block lst;
	block_init(&lst);
	
	p_chunk x = block_newChunk(1,0,0);
	block_insert(&lst, x);
	
	x = block_newChunk(2,1,0);
	block_insert(&lst, x);
	
	x = block_newChunk(3,0,1);
	block_insert(&lst, x);
	
	x = block_newChunk(4,1,1);
	block_insert(&lst, x);
	
	block_print(&lst);
	block_printw(&lst);
	
	REFRESHWAIT;
	
	block_destroy(&lst);
	block_printw(&lst);
	
	REFRESHWAIT;
	
	endwin();
}
