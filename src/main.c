#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "blockstruct.h"
#include "rembrandt.c"
//#include "locknload.h"

#define ERASEFROMBOARD block_wprintw(board, x,y,&lst,0)
#define WRITETOBOARD block_wprintw(board, x,y,&lst,1)

block lst, temp;

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

void titleloop() {
	canvas(0,0);
	refresh();
	mvprintw(13,8,"Game Type A");
	mvprintw(15,8,"Game Type B");
	
	
	
	int selection = 1;
	char ch = 'a';
	
	while (ch) {
		ch = getchar();
		
		if (ch == 'w') {
			mvprintw(13,21, "(*)");
			mvprintw(15,21, "   ");
			refresh();
			selection = 1;
		}
		if (ch == 's') {
			mvprintw(13,21, "   ");
			mvprintw(15,21, "(*)");
			refresh();
			selection = 2;
		}
	}
}

void gameloop() {
	//blockGen();
	//blockToPlay();
	
	int arr[20][10] = {0};
	
	gameWindowInit();
	canvas(3,2);
	layeredRefresh(3);
	
	int x = 5, y = 0;
	
	block_wprintw(board,x,y,&lst,1);
	
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
		
		default: break;
		}
		
		layeredRefresh(1);
		ch = getchar();
	}
	
	
	titleloop();
}


void main() {

	// Required Inits, in order: curses, Rembrandt, current block and block in queue
	initscr();
	rembrandtInit();
	
	// -------------------------------------------
	

	block_init(&lst);
	block_init(&temp);
	
	
	/*// draw blank board
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			mvprintw((i)*2+1,(j)*3+2,".");
		}
	}*/
	
	
	
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

