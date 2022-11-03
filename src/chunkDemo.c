#include <stdlib.h>
#include <stdio.h>

#include "blockstruct.h"
//#include "physics.h"


void main() {
    chunk *D = createChunk(0,1);
    chunk *C = addChunk(2,3,&D);
    chunk *B = addChunk(4,5,&C);
	chunk *A = addChunk(6,7,&B);
	
    chunkList *ABCD = malloc(sizeof(chunkList));
    ABCD->pos = &A;
    
    int *x;
    int *y;

    char ch = getchar();
    
    while (ch != 'e') {
        *x = &ABCD.pos->relativeX;
        *y = &ABCD.pos->relativeY;

        if (ch == 'c') {
            printf("current chunk coordinates: %d %d", &x, &y);
        }
        
        else if (ch == 'v') {
            toNextChunk(&ABCD);
        }
    }
}
