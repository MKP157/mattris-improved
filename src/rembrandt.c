#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "blockstruct.h"

// These macros define the maximum size of the terminal graphics. This isn't concrete,
// but rather a reminder to myself when making the ASCII graphics.
#define MAXLEN 99
#define MAXHEIGHT 51
// ------------------------------------------------------


// One of curses' many limitations lays in how when layering windows, to make changes 
// to any window UNDER the stack, you have to overwrite and refresh every window on
// top. This makes the code likely less readable, and I do apologize for that.


// Globals ----------------------------------------------
WINDOW *stats, *next, *frame, *board;
// ------------------------------------------------------


// Custom curses screen initialization ------------------
void rembrandtInit() {
	
	if ((has_colors() == FALSE) || (can_change_color() == FALSE)) {
                endwin();
      		printf("[!] Mattris requires a terminal with changeable colours.\n");
       		exit(1);
    	}
	start_color();
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
		// Title screen colours
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
		// High scores
	init_pair(3, COLOR_RED, COLOR_BLACK);
		//
	// !!! 4 IS USED BY GRADIENT
}
// -------------------------------------------------------


// Create and format the window for the main game --------
void gameWindowInit(){
	//  next ------------------------------------
	next = newwin(6,24,4,44);
	wborder(next,0,0,0,0,0,0,0,0);
	mvwprintw(next,2,2,"Next up:");
	overwrite(next, stdscr);
	// ------------------------------------------
	
	
	// statistics -------------------------------
	stats = newwin(6,24,13,44);
	wborder(stats,0,0,0,0,0,0,0,0);
		
	overwrite(stats, stdscr);
	// ------------------------------------------
	
	
	// board frame ------------------------------
	frame = newwin(42,32,4,4);
	wborder(frame,0,0,0,0,0,0,0,0);
		
	overwrite(frame, stdscr);
	// ------------------------------------------
	
	
	// board ------------------------------------
	board = newwin(40,30,5,5);
	//wborder(board,0,0,0,0,0,0,0,0);
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 11; j++) {
			mvwprintw(board,(i)*2+1,(j)*3+2,".");
		}
	}
	overwrite(board, frame);
	// ------------------------------------------
}
// ------------------------------------------------------


// Refresh only the necessary windows in the stack ------
void layeredRefresh(int layers) {
	// The stack: --------
	// top 1 : board
	// middle 2 : stats, frame, score
	// bottom 3 : stdscr

	for (int i = 0; i < 2; i++) {
		if (layers > 2) { 
			if (i) refresh(); 
		}
		if (layers > 1) {
			if (!i) { overwrite(next, stdscr); overwrite(stats, stdscr); overwrite(frame, stdscr); }
			else { wrefresh(next); wrefresh(stats); wrefresh(frame); }
		}
		// we always want to refresh board
		if (i) { 
			overwrite(board, frame); 
		}
		else { 
			wrefresh(board); 
		}
	}
}
// ------------------------------------------------------


// Display a template (background) located in any of 
// the GFX files in the graphics folder. ----------------
int canvas(int x, int colour) {
	FILE *load;
	char line[MAXLEN];
	
	switch (x) {
	// Title Screen ------------------
		case 1: load = fopen("../graphics/title.txt", "r"); break;
	// Highscores, or game choice
		case 2: load = fopen("../graphics/bg.txt", "r"); break;
	// Background
		case 3: load = fopen("../graphics/bg.txt", "r"); break;
		
		default:
	}
	
	palette(colour);
	
	if (load == NULL) {
		printf("Bad file!");
		return 0;
	}
     	
     	while (fgets(line, MAXLEN, load))	
     		printw(line);
	
	move(0,0);
	
	fclose(load);
     	refresh();
	
	return 1;
}
// ------------------------------------------------------

void palette(int c) {
	attron(COLOR_PAIR(c));
}

void bmove(int Y, int X) {
	move((Y-1)*2,(X-1)*3);
}

void block_printw(int origX, int origY, p_block plst, int x) {
	p_chunk z = plst->head;
	while(z) {
		move((int)(origY + z->Ry*2), (int)(origX + z->Rx*3));
		//printw("%d", z->k);
		
		
		if (x)  {
			addch(ACS_CKBOARD);addch(ACS_CKBOARD);addch(ACS_CKBOARD);
			move((int)(origY + z->Ry*2 + 1), (int)(origX + z->Rx*3));
			addch(ACS_CKBOARD);addch(ACS_CKBOARD);addch(ACS_CKBOARD);
		}
		else {
			printw("   ");
			move((int)(origY + z->Ry*2 + 1), (int)(origX + z->Rx*3));
			printw("  .");
		}
		z = z->next;
	}
}
