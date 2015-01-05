#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lu.h"

void generateMatrix(int n, double matrix[n][n]);

void ShowUsage ()
{
    printf (
                "Usage:\n"
                "  inverseMatrix  -h | -help | --help   : print help\n"
                "Options:\n"
                "  -n [N]              : generate matrix size N to inverse\n"
                "  -n [N]              : N == 0 start demo\n"
                );
}

int main(int argc, char *argv[])
{
    if ((argc < 2) || strcmp (argv[1], "-h") == 0 || strcmp (argv[1], "-help") == 0 ||
            strcmp (argv[1], "--help") == 0) {
        ShowUsage ();

        return 0;
    }

    if (argc > 2) {
        if (strcmp (argv[1], "-n") == 0 && argc == 3) {
            int n = atoi(argv[2]);

            SquareMatrix *A;
            if (n == 0) {
                double matrixToInverse[4][4] = { {10, 7, 8, 7},
                                                      {7, 5, 6, 5},
                                                      {8, 6, 10, 9},
                                                      {7, 5, 9, 10}
                                                    };

                A = createMatrix(4);
                fillMatrix(A, matrixToInverse);
            }
            else {
                double matrixToInverse[n][n];
                generateMatrix(n, matrixToInverse);

                A = createMatrix(n);
                fillMatrix(A, matrixToInverse);
            }

            //    printMatrixWithName(A, "A");

            SquareMatrix *A_1 = inverse(A);
            //    printMatrixWithName(A_1, "A^(-1)");
            printMatrix(A_1);

            freeMatrix(A);
            freeMatrix(A_1);
        }
    } else {
        ShowUsage ();
    }

    return 0;
}

void generateMatrix(int n, double matrix[n][n])
{
    int i, j;

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            matrix[i][j] = 1.0 / (i + j + 1.0);
        }
    }
}
