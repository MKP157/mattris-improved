#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "blockstruct.h"
#include "rembrandt.c"
//#include "locknload.h"

#define ERASEFROMBOARD block_wprintw(board, x,y,&lst,0)
#define WRITETOBOARD block_wprintw(board, x,y,&lst,1)
#define CLEARREF clear();refresh()

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

void menuloop(int selection) { 
	canvas(selection + 2, selection + 2);
	refresh();
	
	int levelSelection = 0, 
		noiseSelection = 0, 
		cX = 22, 
		cY = 18, 
		menuType = 0;
	
	char ch = 'z';
	
	while ((ch != 13) && (ch != 'e')) {
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
	
	else { gameloop(++levelSelection, ++noiseSelection); }	
}

void titleloop() {
	canvas(1,1);
	mvprintw(13,8,"Game Type A");
	mvprintw(15,8,"Game Type B");
	mvprintw(1,101, "Controls  --------------");
	mvprintw(3,101, "[W]         Rotate Block");
	mvprintw(4,101, "[A]  Move Block Leftward");
	mvprintw(5,101, "[S] Move Block Rightward");
	mvprintw(6,101, "[D]  Move Block Downward");
	mvprintw(8,101, "[Enter]            Pause");
	mvprintw(9,101, "[E]        Quit to Title");
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

void gameloop(int level, int noise) {
	//blockGen();
	//blockToPlay();
	
	int arr[20][10] = {0};
	
	gameWindowInit();
	canvas(4,4);
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

