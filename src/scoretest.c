#include <stdio.h>
#include <stdlib.h>

#include "./scorestruct.h"
#include "./locknload.h"

void main() {
	scoreList r;
	scoreList_init(&r);
	
	p_score A;
	A = scoreList_newScore("ADRI", 4);
	scoreList_insert(&r, A);
	A = scoreList_newScore("AIDE", 1);
	scoreList_insert(&r, A);
	A = scoreList_newScore("JOSH", 6);
	scoreList_insert(&r, A);
	A = scoreList_newScore("JOSH", 6);
	scoreList_insert(&r, A);
	A = scoreList_newScore("JOSH", 6);
	scoreList_insert(&r, A);
	A = scoreList_newScore("nil ", -1);
	scoreList_insert(&r, A);
	
	//scoreList_print((&r)->head);
	
	A = scoreList_newScore("ANSO", 11);
	scoreList_insert(&r, A);
	A = scoreList_newScore("ELON", 9);
	scoreList_insert(&r, A);
	
	printf("\n\n");
	scoreList_print(&r);
	
	
	scoreList x;
	scoreList_init(&x);
	loadScoreData(&x);
	
	printf("\n\n");
	scoreList_print(&x);
	saveScoreData(&x);
	scoreList_delete(&x);
	scoreList_print(&x);
}
