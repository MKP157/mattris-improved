#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "blockstruct.h"

// These macros define the maximum size of the terminal graphics. This isn't concrete,
// but rather a reminder to myself when making the ASCII graphics.
#define MAXLEN 99
#define MAXHEIGHT 51
#define WINDRAWCHECKERROW waddch(win, ACS_CKBOARD); waddch(win, ACS_CKBOARD); waddch(win, ACS_CKBOARD) 
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
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
		// Type A
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
		// Type B
	init_pair(4, COLOR_WHITE, COLOR_BLUE);
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
		case 1: load = fopen("./graphics/title.txt", "r"); break;
	// Type A menu
		case 2: load = fopen("./graphics/typea.txt", "r"); break;
	// Type B menu
		case 3: load = fopen("./graphics/typeb.txt", "r"); break;
	// Type A background
		case 4: load = fopen("./graphics/bg.txt", "r"); break;
	// Type B background
		case 5: load = fopen("./graphics/bg2.txt", "r"); break;
		default: break;
	}
	
	attron(COLOR_PAIR(colour));
	
	if (load == NULL) {
		printf("Bad file!");
		return 0;
	}
     	
     	while (fgets(line, MAXLEN, load))	
     		printw(line);
	
	move(0,0);
	
	fclose(load);
     	refresh();
	attroff(COLOR_PAIR(colour));
	return 1;
}
// ------------------------------------------------------

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

void block_wprintw(WINDOW *win, int origX, int origY, p_block plst, int x) {
	p_chunk z = plst->head;
	while(z) {
		wmove(win, (int)(origY + z->Ry*2), (int)(origX + z->Rx*3));
		//printw("%d", z->k);
		
		
		if (x)  {
			wattron(win, COLOR_PAIR(2));
			WINDRAWCHECKERROW;
			wmove(win, (int)(origY + z->Ry*2 + 1), (int)(origX + z->Rx*3));
			WINDRAWCHECKERROW;
			wattroff(win, COLOR_PAIR(2));
		}
		else {
			wattron(win, COLOR_PAIR(1));
			wprintw(win, "   ");
			wmove(win, (int)(origY + z->Ry*2 + 1), (int)(origX + z->Rx*3));
			wprintw(win, "  .");
			wattroff(win, COLOR_PAIR(1));
		}
		z = z->next;
	}
}

void noise_wprintw(WINDOW *win, int y, int x) {
	wmove(win, y*2, x*3);
	WINDRAWCHECKERROW;
	wmove(win, y*2+1, x*3);
	WINDRAWCHECKERROW;
}
