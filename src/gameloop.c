#include "blockstruct.h"
#include "locknload.h"

#define ERASEFROMBOARD block_wprintw(board,XSCALE*x,YSCALE*y,&lst,0)
#define WRITETOBOARD block_wprintw(board,XSCALE*x,YSCALE*y,&lst,1)
#define CLEARREF clear();refresh()

#define ALARMCANCEL ualarm(0, 0)

#define LEVELTIME (useconds_t)(999999/level)

#define XSCALE 3
#define YSCALE 2

#define T_BLOCK 1
#define I_BLOCK 2
#define O_BLOCK 3
#define J_BLOCK 4
#define L_BLOCK 5
#define S_BLOCK 6
#define Z_BLOCK 7

block lst, q_lst;
int arr[20][10] = {0};
int x = 0, y = 0, level, queue, scoreInstance, lineCount = 0;
int blockCount[7];
int end = 0;

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /$$$$$$$  /$$   /$$ /$$     /$$ /$$$$$$  /$$$$$$  /$$$$$$   /$$$$$$ //////////////////////////////////////////////////////////////////
| $$__  $$| $$  | $$|  $$   /$$//$$__  $$|_  $$_/ /$$__  $$ /$$__  $$//////////////////////////////////////////////////////////////////
| $$  \ $$| $$  | $$ \  $$ /$$/| $$  \__/  | $$  | $$  \__/| $$  \__///////////////////////////////////////////////////////////////////
| $$$$$$$/| $$$$$$$$  \  $$$$/ |  $$$$$$   | $$  | $$      |  $$$$$$ //////////////////////////////////////////////////////////////////
| $$____/ | $$__  $$   \  $$/   \____  $$  | $$  | $$       \____  $$//////////////////////////////////////////////////////////////////
| $$      | $$  | $$    | $$    /$$  \ $$  | $$  | $$    $$ /$$  \ $$//////////////////////////////////////////////////////////////////
| $$      | $$  | $$    | $$   |  $$$$$$/ /$$$$$$|  $$$$$$/|  $$$$$$///////////////////////////////////////////////////////////////////
|__/      |__/  |__/    |__/    \______/ |______/ \______/  \______/ //////////////////////////////////////////////////////////////////
									*/////////////////////////////////////////////////////////////


int collision(char dir, p_block plst, int y, int x) {
	int modX = 0, modY = 0, newX, newY, n;
	
	switch(dir) {
		case 'a':  --modX;  break;
		
		case 'd':  ++modX;  break;
		
		case 's':  ++modY;  break;
		
		default: break;
	}
	
	p_chunk z = plst->head;
	while(z) {
		newX = x + modX + z->Rx;
		newY = y + modY + z->Ry;
		
		if (arr[newY][newX]) return 0;
		if (newX < 0 || newX > 9) return 0;
		if (newY < 0 || newY > 19) return 0;
		z = z->next;
	}
	return 1;
}

int rotation(p_block plst, int y, int x) {
	p_chunk z = plst->head;
	int tempY, tempX, newY, newX, store[8], i = 0;
	
	if (z->k == 3) return 0;
		// [!] O blocks don't need rotation!
		
	while(z) {
		// Clockwise 90deg: (x,y) -> (y,-x), or (y,x) -> (-x,y)
		
		// Center Rx=1,Ry=1 as origin:
		tempY = z->Ry - 1;
		tempX = z->Rx - 1;
		
		newY = y + (-1)*tempX + 1;
		newX = x + tempY + 1;
		
		if (arr[newY][newX]) return 0;
		if ((newX) < 0 || (newX) > 9) return 0;
		if ((newY) < 0 || (newY) > 19) return 0;
		
		// We save the new rotation coordinates for later, because if rotation is found to collide
		// somewhere down the line, we'll have edited the first coordinates and not the rest!
		store[i++] = newY - y;	
		store[i++] = newX - x;
		
		z = z->next;
	}
	
	// Only write the new data to the block if the collision tests are passed
	z = plst->head; i = 0;
	while(z) {
		z->Ry = store[i++];
		z->Rx = store[i++];
		z = z->next;
	}
	
	return 1;
}

void writeBlock(p_block plst) {
	p_chunk z = plst->head;
	int lines[20] = {0}, i = 0;
	
	while(z) {
		arr[y + z->Ry][x + z->Rx] = z->k;
		lines[y + z->Ry]++;
		z = z->next;
	}
	for (i = 0; i < 20; i++) {
		if (lines[i] && checkLine(i)) {
			clearLine(i);
		}
	}
}

int checkLine(int y){
	
	// If any position in the array's row is equal to 0, the line is not filled, and will not clear it.
	for (int i = 0; i < 10; i++) {
		if (!arr[y][i])
			return 0;
	}
	
	lineCount++;
	scoreInstance += 100;
	// If all positions are over 1, the line will clear.
	return y;
}

int clearLine(int y) {
	for(int i = y; i > 0; i--) {
		for (int j = 0; j < 10; j++) {
			arr[i][j] = arr[i-1][j];
		}
	}
	
	lineCount++;
	
	if (lineCount % 15 == 0) {
		level++;
		levelUp();
		printStatUpdate(1, level);
	}
	
	eraseBoard();
	directDraw(10, 1);
}


void newBlock() {
	block_clone(&q_lst, &lst);
	pullBlock(&q_lst);
	
	printNext(&q_lst);
	layeredRefresh(1);
}

int blockOutCheck(p_block plst) {
	p_chunk z = plst->head;
	
	while(z) {
		if(arr[y + z->Ry][x + z->Rx]) return 1;
		z = z->next;
	}
	
	return 0;
}
// Originally intended for generating board noise for type B games,
// however this function also doubles as a board-redraw for the pause
// menu. That's what the "skip" boolean condition is for; it skips
// randomly populating the board for when all that's needed is a skip.

void directDraw(int n, int skip) {
	//printf("Generating noise!");
	int R;
	
	for (int i = 19; i >= 20-(n*2); i--) {
		for (int j = 0; j < 10; j++) {
			
			// If the skip flag = 0; then the function proceeds to generate board noise. This is used for type B games
			if (!skip) {
				R = (rand() % 7);
				if (rand() & 2)
					arr[i][j] = R;
			}
			
			if (arr[i][j]) {
				if (skip == 3)
					noise_wprintw(board,i,j,1);
				else
					noise_wprintw(board,i,j,arr[i][j]);
				// Debug:
				//mvprintw(i,j+100,"%d",arr[i][j]);
			}
		}
	}
	layeredRefresh(3);
}


void sighandler(int signum) {
	ALARMCANCEL;
	
	if (collision('s', &lst, y, x)) {
		ERASEFROMBOARD;
		y++;
		WRITETOBOARD;
		layeredRefresh(1);
		
		ualarm(LEVELTIME, 0);
	}
	
	else {
		writeBlock(&lst);
		x = 3; y = 0;
		
		printStatUpdate(lst.head->k + 2, ++blockCount[lst.head->k - 1]);
		
		newBlock();
		WRITETOBOARD;
		layeredRefresh(1);
		
		if (blockOutCheck(&lst)) {
			blockOut(scoreInstance);
			end = 1;
		}
		else
			ualarm(LEVELTIME, 0);
	}
	
	printStatUpdate(0,scoreInstance);
}

int gameloop(int* level_start, int* noise, int* selection) {
	
	block_init(&lst);
	block_init(&q_lst);
	pullBlock(&q_lst);
	newBlock();
	
	gameWindowInit();
	x = 0, y = 0;
	level = *level_start;
	lineCount = 0;
	
	if (*level_start > 1)
		scoreInstance += *level_start * 1000;
		
	canvas(4 + *selection,COLOR_BLUE - (*selection*2));
	eraseBoard();
	layeredRefresh(3);
	gameunpause();
	
	// Only generate noise if level type is type B
	if (*selection) {
		directDraw(*noise, 0);
		for (int i = 19; i >= 0; i--)
			if (checkLine(i)) 
				clearLine(i);
	}
		
	// Starting position
	// Coordinates work as follows: x or y = (board array pos)*(scale)
	x = 3;
	y = 0;
	WRITETOBOARD;
	signal(SIGALRM,sighandler); // Register signal handler
	ualarm(LEVELTIME, 0);
	
	printStatUpdate(0,scoreInstance);
	printStatUpdate(1,level);
	
	char ch = 'e';
	
	while (ch != 'x' && !end) {
		switch(ch) {
		
		case 'w':
			ERASEFROMBOARD;
			if (rotation(&lst,y,x));
			WRITETOBOARD;
		break;
		
		case 'a':
			if (collision(ch, &lst, y, x)) {
				ERASEFROMBOARD;
				x--;
				WRITETOBOARD;
			}
		break;
		
		case 's':
			scoreInstance += 10;
			sighandler(SIGALRM);	// Trigger the alarm before it's supposed to sound
		break;
		
		case 'd':
			if (collision(ch, &lst, y, x)) {
				ERASEFROMBOARD;
				x++;
				WRITETOBOARD;
			}
		break;
		
		case 13:
			ALARMCANCEL;
			
			// Call the pause function, and upon return from its loop, play the unpause animation
			gamepause();
			gameunpause();
			
			WRITETOBOARD;
			layeredRefresh(3);
			ualarm(LEVELTIME, 0);
		break;
		
		default: 
			ERASEFROMBOARD;
			WRITETOBOARD;
		break;
		}
		
		//for (int i = 0; i < 20; i++) {
		//	for (int j = 0; j < 10; j++) {
		//		mvprintw(i,j+100,"%d",arr[i][j]);
		//	}
		//}
		
		//mvprintw(50,100,"y = %d",y);
		//mvprintw(51,100,"x = %d",x);
		
		layeredRefresh(3);
		ch = getchar();
	}
	
	ALARMCANCEL;
	block_destroy(&lst);
	block_destroy(&q_lst);
	gameWindowExit();
	return 1;
}

