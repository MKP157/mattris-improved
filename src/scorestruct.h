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

void rankList_print(p_rankList plst) {
	if (plst->head != NULL) {
		p_rank z = plst->head;
		rank_print(z);
	}
	
	else printf("Your ranklist is empty or doesn't exist!");
}


void rank_print(p_rank z) {
	if (z->greater != NULL)
		rank_print(z->greater);
	
	printf("score: %010d \n", z->score);
	//getchar();
	//printw("%c%c%c's score: %010d", z->n1, z->n2, z->n3, z->score);
	//getchar();
	
	if (z->lesser != NULL)
		rank_print(z->lesser);
}

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


void loadScoreData(p_rankList plst) {
	FILE *loadScore;
	
	int temp;
	//char n1,n2,n3;
	int len;
	
	loadScore = fopen("./bindata/scoredata_debug.txt", "r");
	
	if (loadScore == NULL) {
		printf("Bad score files!");
		exit(1);
	}
	
	p_rank x;
	for (int i = 0; i < 10; i++) {
		//fscanf(loadScore, "%c", &n1);
		//fscanf(loadScore, "%c", &n2);
		//fscanf(loadScore, "%c", &n3);
		
   		fscanf(loadScore, "%d", &temp);
   		
   		//x = rankList_newRank(n1,n2,n3,temp)
     		x = rankList_newRank(temp);
		rankList_insert(plst, x);
		
		// garbage collection, don't touch
	}
	
	fclose(loadScore);
}

void saveScoreData(p_rankList plst) {
	FILE *saveScore;
	
	int temp;
	char n1,n2,n3;
	int len;
	
	saveScore = fopen("./bindata/scoredata_debug.txt", "w");
	
	if (saveScore == NULL) {
		printf("Bad score files!");
		exit(1);
	}
	
	rankList_fprint(plst, saveScore);
	fclose(saveScore);
	rankList_delete(plst);
}

void rankList_fprint(p_rankList plst, FILE *out) {
	if (plst->head != NULL) {
		p_rank z = plst->head;
		rank_fprint(z, out);
	}
}

void rank_fprint(p_rank z, FILE *out) {
	if (z->greater != NULL)
		rank_fprint(z->greater, out);
	
	//fprintf(out, "%c%c%c %010d\n", z->n1, z->n2, z->n3, z->score);
	fprintf(out, "%010d\n", z->score);
	
	if (z->lesser != NULL)
		rank_fprint(z->lesser, out);
}

// ------------------------------------------------------

#endif

