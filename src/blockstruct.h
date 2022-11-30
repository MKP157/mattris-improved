#include <stdlib.h>

// Include guards
#pragma once
#ifndef BLOCKSTRUCT_H
#define BLOCKSTRUCT_H

// Definitions ------------------------------------------
typedef struct __chunk{
	int k;	// Block key (also determines colour)
			// 1 : T	T_BLOCK 1
			// 2 : I	I_BLOCK 2
			// 3 : O	O_BLOCK 3
			// 4 : J	J_BLOCK 4
			// 5 : L	L_BLOCK 5
			// 6 : S	S_BLOCK 6
			// 7 : Z	Z_BLOCK 7
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

void block_clone(p_block plst,		// from
		 p_block plst2) {	// to
	
	if (plst2 != NULL) block_destroy(plst2);
	block_init(plst2);
	
	p_chunk z = plst->head, x;
	
	while(z) {
		x = block_newChunk(z->k,z->Ry,z->Rx);
		block_insert(plst2,x);
		z = z->next;
	}
}

// TO BE LEFT TO REMBRANDT (kept here for debug only) ---

void block_print(p_block plst) {
	p_chunk z = plst->head;
	while(z) {
		printf("%d %d %d \n", z->k, z->Rx, z-> Ry);
		z = z->next;
	}
}
// ------------------------------------------------------

#endif

