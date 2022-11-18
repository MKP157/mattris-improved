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
	 		if ((temp / 100000000) == block) break;
	}
	
	fclose(load);
	
	return temp;
}

void pullBlock(p_block plst) {
	if (plst != NULL) block_destroy(plst);
	
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

/*int blockToBin(p_block plst) {
	FILE *outfile;
	
	// open file for writing
	outfile = fopen ("person.dat", "w");
	if (outfile == NULL)
	{
 		   fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
	
	// write struct to file
	fwrite (&input1, sizeof(struct p_block), 1, outfile);
 		
	if(fwrite != 0)
 		   printf("contents to file written successfully !\n");
	else
		printf("error writing file !\n");
	 
	// close file
	fclose (outfile);
 
	return 0;
}*/
