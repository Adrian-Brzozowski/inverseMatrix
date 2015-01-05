#include <stdio.h>
#include <stdlib.h>

#include "lu.h"

int main(int argc, char *argv[])
{
    double matrixToInverse[4][4] = { {10, 7, 8, 7},
                                     {7, 5, 6, 5},
                                     {8, 6, 10, 9},
                                     {7, 5, 9, 10}
                                   };

    SquareMatrix *A = createMatrix(4);
    fillMatrix(A, matrixToInverse);
    //    printMatrixWithName(A, "A");

    SquareMatrix *A_1 = inverse(A);
    //    printMatrixWithName(A_1, "A^(-1)");
    printMatrix(A_1);

    freeMatrix(A);
    freeMatrix(A_1);

    return 0;
}
