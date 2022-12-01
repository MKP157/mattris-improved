#include <stdlib.h>
#include <curses.h>

// Include guards
#pragma once
#ifndef SCORESTRUCT_H
#define SCORESTRUCT_H

// Definitions ------------------------------------------
typedef struct __rank{
	//char n1,n2,n3;
	int score;
	
	struct __rank *greater;
	struct __rank *lesser;
} rank;

typedef rank* p_rank;

typedef struct {
	p_rank head;
} rankList;

typedef rankList* p_rankList;

// ------------------------------------------------------

// Functions --------------------------------------------

p_rank rankList_newRank(int in_score) {
	p_rank z = (p_rank)malloc(sizeof(rank));
	
	z->score = in_score;
	return z;
}

void rankList_init(p_rankList plst) { plst->head = NULL; }

void rankList_insert(p_rankList plst, p_rank z) {
	if( plst->head != NULL )
		rank_sort(z, plst->head);
	else
		plst->head = z;
}

void rank_sort(p_rank zN, p_rank zC) {
	if (zN->score >= zC->score && ((zC->greater == NULL) || (zC->greater->score > zN->score))) {
		zN->greater = zC->greater;
		zC->greater = zN;
	}
		
	else if  ((zN->score >= zC->score) && (zC->greater->score < zN->score))
		rank_sort(zN, zC->greater);
		
	else if (zN->score < zC->score && ((zC->lesser == NULL) || (zC->lesser->score < zN->score))) {
		zN->lesser = zC->lesser;
		zC->lesser = zN;
	}
	
	else
		rank_sort(zN, zC->lesser);
}

/*void rankList_print(p_rankList plst) {
	if (plst->head != NULL) {
		p_rank z = plst->head;
		rank_print(z);
	}
	
	else printf("Your ranklist is empty or doesn't exist!");
}


void rank_print(p_rank z) {
	if (z->greater != NULL)
		rank_print(z->greater);
	
	//printf("%c%c%c's score: %010d \n", z->n1, z->n2, z->n3, z->score);
	getchar();
	printw("%c%c%c's score: %010d", z->n1, z->n2, z->n3, z->score);
	getchar();
	
	if (z->lesser != NULL)
		rank_print(z->lesser);
}*/

void rankList_delete(p_rankList plst) {
	if (plst->head == NULL) {
		rankList_init(plst);
		printf("Your ranklist is empty or doesn't exist!");
		return;
	}
	else {
		p_rank z = plst->head;
		rank_delete(z);
		return;
	}
}


void rank_delete(p_rank z) {
	if (z == NULL) return;
	
	rank_delete(z->lesser);
	rank_delete(z->greater);
		
	free(z);

}

// ------------------------------------------------------

#endif

