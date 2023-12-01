#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


int VoteOn(void* Instances[], short nInstances, int nSize) {
    int i, j;
    int maxRepeats = 0;         //stores the number of repeats
    int maxIndex = -1;          //Index of the item in the majority - initialized to -1
    int curRepeats;             //number of repeats of current set of items (i.e i int)

    for(i = 0; i < nInstances; i++) {
        curRepeats = 0;
        for (j = i + 1; j < nInstances; j++) {
            if(memcmp(Instances[i], Instances[j], nSize) == 0) {
                curRepeats++;
            }
        }
        if (curRepeats > maxRepeats) {  //Check if the current repeats is larger than the max allowed repeats
            maxRepeats = curRepeats;    //Max the mac repeats the new current repeats
            maxIndex = i;
        }
    }
    return(maxIndex);
}