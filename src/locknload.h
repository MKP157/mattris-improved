#include <stdio.h>
#include <stdlib.h>

#define DATALEN 8

/*int load(int choice) {
	FILE *fp;
  	fp = fopen("text.txt","r");
  	int result;
  	
  	//fseek(fp, DATALEN * choice, SEEK_SET);
	result = fgets(fp);
	
	fclose(fp);
	
	return result;
}*/

void serialiseWindow(WINDOW* board) {
	FILE *load;
	load = fopen("./bindata/pausewindata.txt", "wb+");
	putwin(board, load);
	fclose(load);
}

void deserializeWindow(WINDOW** board) {
	FILE *load;
	load = fopen("./bindata/pausewindata.txt", "wb+");
	
	WINDOW* win = getwin(load);
  	*board = win;
  	
	wrefresh(*board);
	
	fclose(load);
}
