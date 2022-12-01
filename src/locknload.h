#include <stdio.h>
#include <stdlib.h>

#include "./blockstruct.h"
#include "./scorestruct.h"

#pragma once
#ifndef LOAD_H
#define LOAD_H

// Length of block data: <blocktype,y,x,y,x,y,x,y,x> = 8 integers
#define B_DATALEN 10
#define SC_NAME_DATALEN 10
#define SC_VALU_DATALEN 11
#define OUTFILE_DATALEN 10
int loadBlockData(int block) {
	FILE *load;
	char line[B_DATALEN];
	int temp, temp2;
	
	load = fopen("./bindata/blockdata.txt", "r");
	
	if (load == NULL) {
		printf("Bad block file!");
		exit(1);
	}
	 	
	while (fgets(line, B_DATALEN, load)) {	
	 		temp = atoi(line);
	 		if ((temp / 100000000) == block) break;
	}
	
	fclose(load);
	
	return temp;
}

void pullBlock(p_block plst) {
	if (plst != NULL) block_destroy(plst);
	
	p_chunk x;
	int data = loadBlockData(rand() % 7 + 1);
		// Load a random block from the data file
	
	int k = data / 100000000, Ry, Rx;
	
	for (int i = 0; i < 4; i++) {
		Rx = data % 10; data /= 10;
		Ry = data % 10; data /= 10;
		
		x = block_newChunk(k,Ry,Rx);
		block_insert(plst, x);
	}
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
	for (int i = 0; i < 5; i++) {
		//fscanf(loadScore, "%c", &n1);
		//fscanf(loadScore, "%c", &n2);
		//fscanf(loadScore, "%c", &n3);
		
   		fscanf(loadScore, "%d", &temp);
   		
   		//x = rankList_newRank(n1,n2,n3,temp)
     		x = rankList_newRank(temp);
		rankList_insert(plst, x);
		
		// garbage collection, don't touch
		fscanf(loadScore, "%d", &temp);
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

#endif

