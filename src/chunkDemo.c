#include <stdlib.h>
#include <stdio.h>

#include "blockstruct.h"
//#include "physics.h"


void main() {
    chunk D = {0,1,NULL};
    chunk C = {2,3, &D};
    chunk B = {4,5, &C};
    chunk A = {6,7, &B};

    chunkList ABCD = {&A};
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