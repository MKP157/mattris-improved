
// "load.h" handles all operations where mattris reads,
// writes or creates files.

// C LIBRARY INCLUDES -----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
// ------------------------------------------------------

// CUSTOM LIBRARIES -------------------------------------
#include "linkedlist.h"
// ------------------------------------------------------


// Load a block array from the binary files -------------
chunk BLOCK_LOAD () {
    int CHOICE = rand() % 7;
        // Pick a random piece
    char *s;
    switch(CHOICE) {
		case 0:	// T
		s = "T01101121";
		break;
		
		case 1:	// I
		s = "I01112131";
		break;
		
		case 2: // O
		s = "O11122122";
		break;
		
		case 3:  // J
		s = "J01112120";
		break;
		
		case 4: // L
		s = "L00011121";
		break;
		
		case 5: // S
		s = "S00101121";
		break;
		
		case 6: // Z
		s = "Z01101120";
		break;
        
        default: s = "Z01101120"; break;
    }
    chunk A;

    if (s) {
        A = createChunk();
        chunkAdd(A,s[0],s[1],s[2]);
        chunkAdd(A,s[0],s[3],s[4]);
        chunkAdd(A,s[0],s[5],s[6]);
        chunkAdd(A,s[0],s[7],s[8]);
    }
   
   return A;
   /*TODO: implement file*/
    //return 0;*/
}
// ------------------------------------------------------