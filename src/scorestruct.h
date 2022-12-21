#include <stdlib.h>
#include <curses.h>

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
	
	z->greater = NULL;
	z->lesser = NULL;
	return z;
}

void scoreList_init(p_scoreList plst) { plst->head = NULL; }

void scoreComp(p_score x, p_score z) {
	if (x->s <= z->s) {
		if (x->greater != NULL)
			scoreComp(x->greater, z);
		else
			x->greater = z;
		
	}
	else {
		if (x->lesser != NULL)
			scoreComp(x->lesser, z);
		else
			x->lesser = z;
	}
}

void scoreList_insert(p_scoreList plst, p_score z) {
	if (plst->head == NULL) plst->head = z;
	else {
		p_score x = plst->head;
		scoreComp(x, z);
	}
}


// Finds largest value ------------------------------------------
int scoreList_smallestTraverse(p_score z) {
	if (z->lesser != NULL) return scoreList_smallestTraverse(z->lesser);
	return z->s;
}

int scoreList_smallest(p_scoreList plst) {
	if (plst->head != NULL) {
		p_score z = plst->head;
		return scoreList_smallestTraverse(z);
	}
	return 0;
}
// --------------------------------------------------------------


// Deletion process----------------------------------------------
void scoreList_deleteTraverse(p_score z) {
	if (z->greater != NULL) { scoreList_deleteTraverse(z->greater); z->greater = NULL; }
	if (z->lesser != NULL)  { scoreList_deleteTraverse(z->lesser); z->lesser = NULL; }
	free(z);
}

void scoreList_delete(p_scoreList plst) {
	if (plst->head != NULL) {
		p_score z = plst->head;
		scoreList_deleteTraverse(z);
		plst->head = NULL;
	}
	free(plst->head);
}
// --------------------------------------------------------------

// Printing to terminal------------------------------------------
void scoreList_printTraverse(p_score z) {
	if (z->greater != NULL) scoreList_printTraverse(z->greater);
	printf("%.4s : %010d\n", z->name, z->s);
	if (z->lesser != NULL) scoreList_printTraverse(z->lesser);
}

void scoreList_print(p_scoreList plst) {
	if (plst->head != NULL) {
		p_score z = plst->head;
		scoreList_printTraverse(z);
	}
	// else printf("Nothing here, chief.\n");
}
// --------------------------------------------------------------


// For saving to files in order ---------------------------------
void scoreList_fprintTraverse(p_score z, FILE *save, int *count) {
	if (z->greater != NULL) scoreList_fprintTraverse(z->greater, save, count);
	if ((*count)++ < 10) fprintf(save, "%.4s : %010d\n", z->name, z->s);
	if (z->lesser != NULL) scoreList_fprintTraverse(z->lesser, save, count);
}

void scoreList_fprint(p_scoreList plst, FILE *save) {
	if (plst->head != NULL) {
		int len = 0;
		p_score z = plst->head;
		scoreList_fprintTraverse(z, save, &len);
	}
}
// --------------------------------------------------------------


// For writing to the titlescreen in order ----------------------

void scoreList_wprintTraverse(p_score z, int *y, int *x, int *pos) {
	if (z->greater != NULL) scoreList_wprintTraverse(z->greater, y, x, pos);
	mvprintw((*y)++, *x, "[#%02d]  %.4s's score :", (*pos)++, z->name);
	mvprintw((*y)++, *x, "@|||@  %010d points", z->s);
	mvprintw((*y)++, *x, " ||| ");
	if (z->lesser != NULL) scoreList_wprintTraverse(z->lesser,  y, x, pos);
}

void scoreList_wprint(p_scoreList plst, int STARTY, int STARTX) {
	if (plst->head != NULL) {
		p_score z = plst->head;
		int count = 1;
		scoreList_wprintTraverse(z, &STARTY, &STARTX, &count);
	}
}
// --------------------------------------------------------------
#endif

