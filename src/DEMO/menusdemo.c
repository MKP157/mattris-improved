#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

// These macros define the maximum size of the terminal graphics. This isn't concrete,
// but rather a reminder to myself when making the ASCII graphics.
#define MAXLEN 99
#define MAXHEIGHT 51
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
}

// Display a template (background) located in any of 
// the GFX files in the graphics folder. ----------------
int canvas(int x) {
	FILE *load;
	char line[MAXLEN];
	
	switch (x) {
	// Title Screen ------------------
		case 1:
		load = fopen("../graphics/title.txt", "r");
		attron(COLOR_PAIR(x));
		break;
	// Highscores, or game choice
		case 2:
		load = fopen("../graphics/bg.txt", "r");
		break;
		case 3:
		load = fopen("../graphics/bg.txt", "r");
		attron(COLOR_PAIR(x));
		break;
		default:
	}
	
	
	if (load == NULL) {
		printf("Bad file!");
		return 0;
	}
     	
     	// Gradient test
     	if (x == 2) {
     	
     	}
     	
     	else {	while (fgets(line, MAXLEN, load)) {	printw(line);	}	}
	
	move(0,0);
	
	fclose(load);
     	refresh();
	
	return 1;
}
// ------------------------------------------------------

WINDOW* drawBoard() {
	WINDOW *B;
	int w = 34, h = 44;
	B = newwin(44,34,5,10);
	
	char X = 97;
	for (int i = 0; i < w; i++) {
		for (int j 
	 
	mvwprint(B,)	
}
