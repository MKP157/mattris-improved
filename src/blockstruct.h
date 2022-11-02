#include <stdlib.h>

struct chunk {
    int relativeX;
    int relativeY;
    chunk *next;
    //chunk *prev;
};

typedef struct  {
    char type;
    chunk *pos;
} chunkList;

void toNextChunk(chunkList *x) {
    if (x->pos->next != NULL) {
        x->pos = x->pos->next;
    }
    else
        printf("chunkList exhausted!");
}