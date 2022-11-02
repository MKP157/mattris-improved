#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

// Creation of the block LinkedList type ----------------
struct block{
    int RELATIVE_Y, RELATIVE_X;
    char TYPE;
    struct block *NEXT;
};

typedef struct block *chunk; 
    //Define chunk as pointer of data type struct block
// ------------------------------------------------------


// Declare the first chunk of a block -------------------
chunk createChunk() {
    chunk X; 
        // Declare a chunk
    X = (chunk)malloc(sizeof(struct block)); 
        // Allocate necessary memory using malloc()
    X->NEXT = NULL;
        // Point following block to nothing
    return X;
        //Return the new chunk
}
// ------------------------------------------------------


// Add a chunk to a block -------------------------------
chunk addChunk(chunk inHEAD, char inTYPE, int inY, int inX) {
    chunk TEMP, Z;
        // Declare two chunks TEMP and Z
    TEMP = createChunk(inTYPE, inY, inX);
        // createChunk will return a new chunk with position data and NEXT pointing to NULL.
    
    // Add positional value to the generated chunk
    TEMP->RELATIVE_Y = inY; 
    TEMP->RELATIVE_X = inX;
    TEMP->TYPE = inTYPE;
    
    // When linked list is empty:
    if (inHEAD == NULL) {
        inHEAD = TEMP;     
    }
    
    // When linked list has elements:
    else {
        Z  = inHEAD;
            // Assign head to Z
        while (Z->NEXT != NULL) {
            Z = Z->NEXT;
                //T raverse the list until Z is the last chunk. The last chunk always points to NULL.
        }
        Z->NEXT = TEMP;
            //Point the previous last chunk to the new chunk created.
    }
    return inHEAD;
}
// ------------------------------------------------------

