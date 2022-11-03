#include <stdlib.h>

struct chunk {
    int relativeX;
    int relativeY;
    struct chunk *next;
    //chunk *prev;
} chunk;

typedef struct  {
    char type;
    chunk *pos;
} chunkList;

chunk createChunk(int x, int y) {
	
	chunk temp;
	temp = malloc(sizeof(chunk));
	temp.relativeY = y;
	temp.relativeX = x;
	temp.next = NULL;
	
	return temp;
}

chunk addChunk(int x, int y, chunk *z) {
	
	chunk temp;
	temp = malloc(sizeof(chunk));
	temp.relativeY = y;
	temp.relativeX = x;
	temp.next = &z;
	
	return temp;
}

void toNextChunk(chunkList *x) {
    if (x->pos->next != NULL) {
        x->pos = x->pos->next;
    }
    else
        printf("chunkList exhausted!");
}
