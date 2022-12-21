#include <stdio.h>
#include <stdlib.h>

#include "./blockstruct.h"
#include "./scorestruct.h"

#pragma once
#ifndef LOAD_H
#define LOAD_H

// Length of block data: <blocktype,y,x,y,x,y,x,y,x> = 8 integers
#define B_DATALEN 10
#define S_DATALEN 19
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

void loadScoreData(p_scoreList plst) {
	FILE *load;
	char line[S_DATALEN];
	int temp;
	
	load = fopen("./bindata/scoredata.txt", "r");
	
	if (load == NULL) {
		printf("Bad score file!");
		exit(1);
	}
	
	p_score A;
	
	while (fgets(line, S_DATALEN, load)) {	
		char temp[4] = {line[0], line[1], line[2], line[3]};
		char* temp2 = line + 7;
		int s = atoi(temp2);
		
		A = scoreList_newScore(temp, s);
		scoreList_insert(plst, A);
	}
	
	fclose(load);
}

void saveScoreData(p_scoreList plst) {
	FILE *save;
	
	save = fopen("./bindata/scoredata.txt", "w");
	
	if (save == NULL) {
		printf("Bad score file!");
		exit(1);
	}
	
	scoreList_fprint(plst,save);
	
	fclose(save);
}

#endif

