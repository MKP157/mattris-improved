// Libraries:
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

// Custom includes:
#include "blockstruct.h"
#include "locknload.h"

// Numerical Defines --------------------------------------------------------------------
#define XSCALE 3
#define YSCALE 2

// These macros define the maximum size of the terminal graphics. This isn't concrete,
// but rather a reminder to myself when making the ASCII graphics.
#define MAXLEN 99	
#define MAXHEIGHT 51
#define WINDRAWCHECKERROW waddch(win, ACS_CKBOARD); waddch(win, ACS_CKBOARD); waddch(win, ACS_CKBOARD)
// --------------------------------------------------------------------------------------


// Inverted Colors! ---------------------------------------------------------------------
#define INV_COLOR_BLACK 8
#define INV_COLOR_RED 9
#define INV_COLOR_GREEN 10
#define INV_COLOR_YELLOW 11
#define INV_COLOR_BLUE 12
#define INV_COLOR_MAGENTA 13
#define INV_COLOR_CYAN 14
#define INV_COLOR_WHITE 15
// --------------------------------------------------------------------------------------


// Globals ------------------------------------------------------------------------------
WINDOW *stats, *next, *frame, *board;
// --------------------------------------------------------------------------------------


// Custom curses screen initialization --------------------------------------------------
void rembrandtInit() {
	
	if ((has_colors() == FALSE) || (can_change_color() == FALSE)) {
            endwin();
      		printf("[!] Mattris requires a terminal with changeable colours.\n");
       		exit(1);
    }
	
	start_color();
	
	// Custom colour definitions, because I quite honestly despise the default GNOME terminal colours.
	// These are much more vibrant, and sourced from https://www.december.com/html/spec/colorper.html.
	init_color(COLOR_RED, 990,80,0);			// Raspberry
	init_color(COLOR_GREEN, 0,1000,0);			// Lime
	init_color(COLOR_YELLOW, 850,850,100);		// Gold
	init_color(COLOR_BLUE, 0,700,930);			// Sky Blue
	init_color(COLOR_MAGENTA, 1000,0,1000);		// Fuschia
	
	// Initialize colour pairs as the name integer value of their 
	// respective colours, for simplicity. COLOR_<type> ( BLUE, RED, etc)
	// is a macro defined by the curses library, with integer values.
	// Int value legend: https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
	for (int cMacro = 1; cMacro < 8; cMacro++) {
		init_pair(cMacro, cMacro, COLOR_BLACK);		// Color on black
		init_pair(cMacro + 8, COLOR_WHITE, cMacro);	// White on color
	}
}
// ---------------------------------------------------------------------------------------


// Create and format the window for the main game ----------------------------------------
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
// --------------------------------------------------------------------------------------


// Refresh only the necessary windows in the stack --------------------------------------

void layeredRefresh(int layers) {
// One of curses' many limitations lays in how when layering windows, to make changes 
// to any window UNDER the stack, you have to overwrite and refresh every window on
// top. This makes the code likely less readable, and I do apologize for that.

// The stack:
// 	top 1 : board
// 	middle 2 : stats, frame, score
// 	bottom 3 : stdscr

	if (layers >= 3) { wnoutrefresh(stdscr); }
	if (layers >= 2) { wnoutrefresh(next); wnoutrefresh(stats); wnoutrefresh(frame); }
	if (layers) { wnoutrefresh(board); }
	
	doupdate();
	// Uses only one doupdate(), as per the documentation.
	// This is much easier on CPU cycles. https://linux.die.net/man/3/doupdate
}
// --------------------------------------------------------------------------------------


// Display a template (background) located in any of 
// the GFX files in the graphics folder. ------------------------------------------------
int canvas(int x, int colour) {
	FILE *load;
	char line[MAXLEN];
	
	switch (x) {
	// Title Screen
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
// --------------------------------------------------------------------------------------

void bmove(int Y, int X) {
	move((Y-1)*2,(X-1)*3);
}

/*void block_printw(int origX, int origY, p_block plst, int x) {
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
}*/

void block_wprintw(WINDOW *win, int origX, int origY, p_block plst, int x) {
	p_chunk z = plst->head;
	while(z) {
		wmove(win, (int)(origY + z->Ry*2), (int)(origX + z->Rx*3));
		//printw("%d", z->k);
		
		
		if (x)  {
			wattron(win, COLOR_PAIR(z->k));
				// Change colour to that stored in block
			WINDRAWCHECKERROW;
				// Draw row of checkerboards
			wmove(win, (int)(origY + z->Ry*2 + 1), (int)(origX + z->Rx*3));	
				// Move to next row
			WINDRAWCHECKERROW;
				// Draw row of checkerboards
			wattroff(win, COLOR_PAIR(z->k));
				// Disable block colour
		}
		else {
			wattron(win, COLOR_PAIR(COLOR_WHITE));
			wprintw(win, "   ");
			wmove(win, (int)(origY + z->Ry*2 + 1), (int)(origX + z->Rx*3));
			wprintw(win, "  .");
			wattroff(win, COLOR_PAIR(COLOR_WHITE));
		}
		z = z->next;
	}
}

void noise_wprintw(WINDOW *win, int y, int x) {
	int randomColour = rand() % 7 + 1;
	
	wattron(win, COLOR_PAIR(randomColour));
		wmove(win, y*YSCALE, x*XSCALE);
		WINDRAWCHECKERROW;
		wmove(win, y*YSCALE+1, x*XSCALE);
		WINDRAWCHECKERROW;
	wattroff(win, COLOR_PAIR(randomColour));
}

void eraseBoard() {
	wattron(board, COLOR_PAIR(COLOR_WHITE));
	wclear(board);
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 11; j++) {
			mvwprintw(board,(i)*2+1,(j)*3+2,".");
		}
	}
	wattroff(board, COLOR_PAIR(COLOR_WHITE));
}

// Some defines specific to the pause system --------------------------------------------
#define BOARDREF wrefresh(board);
#define BOARD_U_SL_REF  BOARDREF; usleep(100000);
#define BOARD_SL_REF  BOARDREF; sleep(1);
// --------------------------------------------------------------------------------------

// Game-pause menu ----------------------------------------------------------------------
void gamepause() {
	int temp;
	char ch2 = 'a';
	while (ch2 != 13) {
	
		// Here's a fun little feature: with each button press on the pause
		// menu, the pause graphic will change colour randomly.
		temp = rand() % 6 + 9;
		attron(COLOR_PAIR(temp));
		mvprintw(0,2,"88\"\"Yb    db    88   88 .dP\"Y8 888888 ");
		mvprintw(1,2,"88__dP   dPYb   88   88 `Ybo.\" 88__   ");
		mvprintw(2,2,"88\"\"\"   dP__Yb  Y8   8P o.`Y8b 88\"\"   ");
		mvprintw(3,2,"88     dP\"\"\"\"Yb `YbodP\' 8bodP\' 888888 ");
		
		attroff(COLOR_PAIR(temp));
		layeredRefresh(3);
		
		ch2 = getchar();
	} 
	for (int i = 0;i<4;)
		mvprintw(i++,2,"                                        ");
}
// ------------------------------------------------------

// Animation sequence for the unpause function ----------
void gameunpause() {
	
	int x = 11;
	wattron(board, COLOR_PAIR(COLOR_RED));
	
	mvwprintw(board, 12, x, " :::::::: ");
	mvwprintw(board, 13, x, ":+:    :+:");
	mvwprintw(board, 14, x, "       +:+");
	mvwprintw(board, 15, x, "    +#++: ");
	mvwprintw(board, 16, x, "       +#+");
	mvwprintw(board, 17, x, "#+#    #+#");
	mvwprintw(board, 18, x, " ######## ");
	BOARD_SL_REF;
	
	mvwprintw(board, 12, x, " :::::::: ");
	mvwprintw(board, 13, x, ":+:    :+:");
	mvwprintw(board, 14, x, "      +:+ ");
	mvwprintw(board, 15, x, "    +#+   ");
	mvwprintw(board, 16, x, "  +#+     ");
	mvwprintw(board, 17, x, " #+#      ");
	mvwprintw(board, 18, x, "##########");
	BOARD_SL_REF;
	
	mvwprintw(board, 12, x, "    :::   ");
	mvwprintw(board, 13, x, "  :+:+:   ");
	mvwprintw(board, 14, x, "    +:+   ");
	mvwprintw(board, 15, x, "    +#+   ");
	mvwprintw(board, 16, x, "    +#+   ");
	mvwprintw(board, 17, x, "    #+#   ");
	mvwprintw(board, 18, x, "  ####### ");
	BOARD_SL_REF;
	
	wattroff(board, COLOR_PAIR(COLOR_RED));
	
	for (int i = 0; i < 3; i++) {
		wattron(board, COLOR_PAIR(COLOR_GREEN));
		mvwprintw(board, 12, 0, " ::::::::   ::::::::  ::: ::: ");
		mvwprintw(board, 13, 0, ":+:    :+: :+:    :+: :+: :+: ");
		mvwprintw(board, 14, 0, "+:+        +:+    +:+ +:+ +:+ ");
		mvwprintw(board, 15, 0, ":#:        +#+    +:+ +#+ +#+ ");
		mvwprintw(board, 16, 0, "+#+   +#+# +#+    +#+ +#+ +#+ ");
		mvwprintw(board, 17, 0, "#+#    #+# #+#    #+#         ");
		mvwprintw(board, 18, 0, " ########   ########  ### ### ");
		wattroff(board, COLOR_PAIR(COLOR_GREEN));
		
		BOARD_U_SL_REF;
		
		for (int j = 12; j < 19;) {
			mvwprintw(board, j++, 0, "                              ");
			mvwprintw(board, j++, 0, "  .  .  .  .  .  .  .  .  .  .");
		}
		
		BOARD_U_SL_REF;
	}
	generateNoise(10,1);
	layeredRefresh(3);
}
// ------------------------------------------------------


