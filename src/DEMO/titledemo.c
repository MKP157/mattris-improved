#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "../rembrandt.c"

#define MAXLEN 99
#define MAXHEIGHT 51

int main() {
	initscr();
	rembrandtInit();
	canvas(1);
	getchar();
	
	//WINDOW *board;
	//board = create_newwin(40,30,10,10);
	//draw_borders(board);
	
	canvas(2);
	getchar();
	
	canvas(3);
	getchar();
	
	endwin();
	/*initscr();
	
	FILE *textfile;
	char line[MAXLEN];
	     
	textfile = fopen("../graphics/title", "r");
	
	if (textfile == NULL) {
		printf("Bad file!");
		return 1;
	}
     	
     	int counter = 0;
     	while (fgets(line, MAXLEN, textfile)) {
		printw(line);
	}
     
	fclose(textfile);
	
	refresh();
	getchar();
	
	endwin();
	return 0;*/
}
