#include <stdio.h>
#include <stdlib.h>

#define DATALEN 8

int load(int choice) {
	FILE *fp;
  	fp = fopen("text.txt","r");
  	int result;
  	
  	//fseek(fp, DATALEN * choice, SEEK_SET);
	result = fgets(fp);
	
	fclose(fp);
	
	return result;
}
