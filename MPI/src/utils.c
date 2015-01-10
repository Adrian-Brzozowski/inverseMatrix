#include "utils.h"

void divideLoop(int range, int *iLoopProc) {
    int i, j;
    int chunk;
    int procTest;
    int iMin, iMax;
    chunk = range / numprocs;
    procTest = range % numprocs;
    /* divide loop */
    for (i = 0; i < numprocs; ++i) {
        iMin = chunk * i + (procTest < i ? procTest : i);
        iMax = iMin + chunk + (procTest > i);
        for (j = iMin; j < iMax; ++j)
            iLoopProc[j] = i;
    }
}
