#ifndef UTILS_H
#define UTILS_H

#include <mpi/mpi.h>

#define ROOT_ID 0
int numprocs, my_rank;
MPI_Status status;

void divideLoop(int range, int *iLoopProc);
#endif
