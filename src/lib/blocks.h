
// "blocks.h" handles all operations where blocks are
// accessed in mattris. This includes rendering them
// with curses, as well as their physics.

// C LIBRARY INCLUDES -----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
// ------------------------------------------------------

// CUSTOM LIBRARIES -------------------------------------
#include "linkedlist.h"
// ------------------------------------------------------


// MACROS -----------------------------------------------
#define ANCHOR_RETURN move(ANCHOR_Y, ANCHOR_X)
	// Returns cursor to anchorpoint.
// ------------------------------------------------------


// Initialize the anchor points, and the block ----------
int ANCHOR_Y = 0;
int ANCHOR_X = 0;

chunk BLOCK_CURRENT, BLOCK_NEXT;
// ------------------------------------------------------


// Generate active block data---------------------------------
void BLOCK_RETRIEVE() {
	// If the next block is empty, generate on the fly. This would be the game's starting position.
	if (BLOCK_NEXT = NULL)
		BLOCK_CURRENT = BLOCK_LOAD();
	else
		BLOCK_CURRENT = BLOCK_NEXT;
	
	BLOCK_NEXT = BLOCK_LOAD();
}
// ------------------------------------------------------


// Draws block in relation to the block-anchor. ---------
void BLOCK_DRAW(int OPERATION) {
	int *cY = &ANCHOR_Y, *cX = &ANCHOR_X;
	char OP_RESULT;

	ANCHOR_RETURN;
	
	while (BLOCK_CURRENT != NULL) {
		printf("%p", BLOCK_CURRENT);
		BLOCK_CURRENT = BLOCK_CURRENT->NEXT;
	}

	ANCHOR_RETURN;
}
// ------------------------------------------------------

/* list traversal
chunk p;
p = head;
while(p != NULL){
    p = p->next;
}
*/