#include <stdio.h>
#include <stdlib.h>

// Score Data Type  -------------------------------------
typedef struct {
	unsigned int score;
	unsigned int blockOccurance[7];
		// How many of each block occured in a given game.
		// [0] = T, [1] = I, [2] = 0, [3] = J, [4] = L, [5] = S, [6] = Z
		// ex: when T occurs, blockOccurance[0]++;
	char name[8];
} playerScore;
// ------------------------------------------------------


