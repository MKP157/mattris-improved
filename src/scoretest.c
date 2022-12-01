#include <stdio.h>
#include <stdlib.h>

#include "./locknload.h"
#include "./scorestruct.h"


void main() {
	rankList r;
	rankList_init(&r);
	
	/*p_rank x;
	
	x = rankList_newRank("MATTKP  ", 8);
	rankList_insert(&r, x);
	
	x = rankList_newRank("ADRIANFR", 4);
	rankList_insert(&r, x);
	
	x = rankList_newRank("AIDENMAN", 1);
	rankList_insert(&r, x);
	
	x = rankList_newRank("JOSHBACO", 6);
	rankList_insert(&r, x);
	
	printf("\n\n");
	rankList_print(&r);
	
	x = rankList_newRank("ANSONGRE", 11);
	rankList_insert(&r, x);
	
	x = rankList_newRank("ELONMUH ", 9);
	rankList_insert(&r, x);
	
	printf("\n\n");
	rankList_print(&r);
	
	printf("\n\n");
	rankList_delete(&r);*/
	
	loadScoreData(&r);
	
	rankList_print(&r);
	
	saveScoreData(&r);
}
