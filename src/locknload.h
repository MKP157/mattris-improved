#include <stdio.h>
#include <stdlib.h>

#include "blockstruct.h"

// Length of block data: <blocktype,y,x,y,x,y,x,y,x> = 8 integers
#define B_DATALEN 10

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
     		printf("%d\n", temp);
     		
     		if ((temp / 100000000) == block) break;
	}
	
	fclose(load);
	
	return temp;
}

void pullBlock(p_block plst) {
	p_chunk x;
	int data = loadBlockData(rand() % 6 + 1);
		// Load a random block from the data file
	
	int k = data / 100000000, Ry, Rx;
	
	for (int i = 0; i < 4; i++) {
		Rx = data % 10; data /= 10;
		Ry = data % 10; data /= 10;
		
		x = block_newChunk(k,Ry,Rx);
		block_insert(plst, x);
	}
}
