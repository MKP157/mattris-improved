#include <stdlib.h>

#include "blockstruct.h"


//int[20][10] createBoard() {
//	int z[20][10] = {0};
//	return z;
//}

int mvblock(char* control, int* anchor[2], int* board[20][10], p_block plst) {
	int result = 1;
	int modX = 0, modY = 0, n;
	p_chunk z = plst->head;
	
	// Movement collision tests (left, right, or down) --------------------
	if (*control != 'w') {
		switch(*control) {
			case 'a': modX--; break;
		
			case 'd': modX++; break;
			
			//down
			default: modY++; break;
		}
		
		// Loop through block's chunks' coordinates:
		int Cy = 0, Cx = 0;
		z = plst->head;
		while(z) {
			// Collision-check coordinates: Relative chunk coordinate + Anchor coordinate + Modifier
			Cy = z->Ry + *anchor[0] + modY;
			Cx = z->Rx + *anchor[1] + modX;
			
			if (*board[Cy][Cx] != 0) // ******THIS COULD BE NULL
				result = 0;
			
			// Escape, and return 0, if the block collides with another solid
			if (!result) break;
			
			z = z->next;
		}
	}
	
	// Rotation and its associated collison -------------------------------
	else {
		
	}
	
	/*
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (BLOCK_ARRAY[i][j]) {
				if ((ANCHOR_X+j+modX != 10) && (ANCHOR_X+j+modX != -1) && (ANCHOR_Y+i+modY != 20)) {
					n = BLOCK_ARRAY[i][j] + GAMEBOARD[ANCHOR_Y+i+modY][ANCHOR_X+j+modX];
					result = result && (n != 2);
				}
				
				else result = 0;
			}
			if (!result) break;
		}
		if (!result) break;
	}*/
	
	// Drawing the block -------------------------------------------------
	if (result) {
		// Erase
		z = plst->head;
		while(z) {
			move(*anchor[0] + z->Ry, *anchor[1] + z->Rx);
			printw("   \n  .");
			
			z = z->next;
		}
		
		// Write
		z = plst->head;
		while(z) {
			move(*anchor[0] + z->Ry + modY, *anchor[1] + z->Rx + modX);
			addch(ACS_CKBOARD); 
			addch(ACS_CKBOARD); 
			addch(ACS_CKBOARD);
			printw("\n");
			addch(ACS_CKBOARD);
			addch(ACS_CKBOARD);
			addch(ACS_CKBOARD);
			
			z = z->next;
		}
	}
}
