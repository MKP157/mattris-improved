#include <stdio.h>
#include <stdlib.h>

#include "blockstruct.h"
#include "scorestruct.h"

#pragma once
#ifndef LOAD_H
#define LOAD_H

// Length of block data: <blocktype,y,x,y,x,y,x,y,x> = 8 integers
#define B_DATALEN 10
#define SC_NAME_DATALEN 10
#define SC_VALU_DATALEN 11

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
	char n1,n2,n3;
	int len;
	
	loadScore = fopen("./bindata/scoredata.txt", "r");
	
	if (loadScore == NULL) {
		printf("Bad score files!");
		exit(1);
	}
	
	p_rank x;
	for (int i = 1; i < len; i++) {
		fscanf(loadScore, "%c", &n1);
		fscanf(loadScore, "%c", &n2);
		fscanf(loadScore, "%c", &n3);
		
   		fscanf(loadScore, "%d", &temp);
   		
   		x = rankList_newRank(n1,n2,n3,temp);
		rankList_insert(plst, x);
		
		// garbage collection, don't touch
		fscanf(loadScore, "%d", &temp);
	}
	
	fclose(loadScore);
}

/*void saveScoreData(p_rankList plst) {
	FILE *loadScore;
	
	int temp;
	char n1,n2,n3;
	int len;
	
	loadScore = fopen("./bindata/scoredata.txt", "w");
	
	if (loadScore == NULL) {
		printf("Bad score files!");
		exit(1);
	}
	
	fprintf(loadScore, "%d", &len);
	
	p_rank x;
	for (int i = 0; i < 5; i++) {
		fscanf(loadScore, "%c", &n1);
		fscanf(loadScore, "%c", &n2);
		fscanf(loadScore, "%c", &n3);
		
   		fscanf(loadScore, "%d", &temp);
   		
   		x = rankList_newRank(n1,n2,n3,temp);
		rankList_insert(plst, x);
		
		fscanf(loadScore, "%d", &temp);
	}
	
	fclose(loadScore);
}*/

#endif

