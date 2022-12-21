#include <stdlib.h>

// Include guards
#pragma once
#ifndef scoreListSTRUCT_H
#define scoreListSTRUCT_H

// Definitions ------------------------------------------
typedef struct __score{
	int s;
	char name[4];
	
	struct __score *greater;
	struct __score *lesser;
} score;

typedef score* p_score;

typedef struct {
	p_score head;
} scoreList;

typedef scoreList* p_scoreList;

// ------------------------------------------------------

// Functions --------------------------------------------

// Create a new scoreList
p_score scoreList_newScore(char n[], int key) {
	p_score z = (p_score)malloc(sizeof(score));
	z->s = key;
	int i = 0;
	for (; n[i] && i < 4; i++)
		z->name[i] = n[i];
	z->name[4] = '\0';
	
	return z;
}

void scoreList_init(p_scoreList plst) { plst->head = NULL; }

void scoreComp(p_score x, p_score z) {
	if (x->s >= z->s) {
		if (x->lesser != NULL)
			scoreComp(x->lesser, z);
		else
			x->lesser = z;
	}
	else {
		if (x->greater != NULL)
			scoreComp(x->greater, z);
		else
			x->greater = z;
	}
}

void scoreList_insert(p_scoreList plst, p_score z) {
	if (plst->head == NULL) plst->head = z;
	else {
		p_score x = plst->head;
		scoreComp(x, z);
	}
}

void scoreList_print(p_scoreList plst) {
	if (plst->head != NULL) {
		p_score z = plst->head;
		scoreList_printTraverse(z);
	}
}

void scoreList_printTraverse(p_score z) {
	if (z->lesser != NULL) scoreList_printTraverse(z->lesser);
	printf("%.4s : %010d\n", z->name, z->s);
	if (z->greater != NULL) scoreList_printTraverse(z->greater);
}

void scoreList_fprint(p_scoreList plst, FILE *save) {
	if (plst->head != NULL) {
		p_score z = plst->head;
		scoreList_fprintTraverse(z, save);
	}
}

void scoreList_fprintTraverse(p_score z, FILE *save) {
	if (z->lesser != NULL) scoreList_fprintTraverse(z->lesser, save);
	fprintf(save, "%.4s : %010d\n", z->name, z->s);
	if (z->greater != NULL) scoreList_fprintTraverse(z->greater, save);
}
#endif

