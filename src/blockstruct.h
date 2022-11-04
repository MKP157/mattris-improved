#include <stdlib.h>

// Definitions ------------------------------------------
typedef struct __chunk{
    int k;	// Position key for debugging.
    int Rx;	// Placement in block structure, relative to the anchor's x.
    int Ry;	// Placement in block structure, relative to the anchor's y.
    
    struct __chunk *next;
    struct __chunk *prev;
} chunk;

typedef chunk* p_chunk;

typedef struct {
    p_chunk head;
} block;

typedef block* p_block;

// ------------------------------------------------------

// Functions --------------------------------------------

// Create a new block
p_chunk block_newChunk(int KEY, int temp_Rx, int temp_Ry) {
	p_chunk z = (p_chunk)malloc(sizeof(chunk));
	z->k = KEY;
	z->Rx = temp_Rx;
	z->Ry = temp_Ry;
	return z;
}

void block_init(p_block plst) { plst->head = NULL; }

void block_insert(p_block plst, p_chunk z) {
	z->next = plst->head;
	
	if (plst->head != NULL)
		plst->head->prev = z;
		
	plst->head = z;
}

void block_print(p_block plst) {
	p_chunk z = plst->head;
	while(z) {
		printf("%d %d %d \n", z->k, z->Rx, z-> Ry);
		z = z->next;
	}
}

void block_printw(p_block plst) {
	p_chunk z = plst->head;
	while(z) {
		move(z->Ry, z->Rx);
		printw("%d", z->k);
		z = z->next;
	}
}

p_chunk block_delete(p_block plst, p_chunk z) {
	if (z->prev != NULL) {
		z->prev->next = z->next;
	}
	else {
		plst->head = z->next;
	}
	if (z->next != NULL) {
		z->next->prev = z->prev;
	}
	free(z);
}

void block_destroy(p_block plst) {
	p_chunk z = plst->head;
	while(z) {
		block_delete(plst, z);
		z = z->next;
	}
}


