#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include <mpi/mpi.h>
#include "utils.h"

#include "lu.h"

void generateMatrix(int n, long double *matrix[]);
void saveResults(int size, struct timeval begin, struct timeval end);
void ShowUsage ()
{
    if (my_rank != ROOT_ID) return;

    printf (
                "Usage:\n"
                "  inverseMatrix  -h | -help | --help   : print help\n"
                "Options:\n"
                "  -n [N]           : generate matrix size N to inverse\n"
                "  -n [N]           : N == 0 start demo\n"
                );
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

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
                long double matrixToInverse[4][4] = { {10, 7, 8, 7},
                                                      {7, 5, 6, 5},
                                                      {8, 6, 10, 9},
                                                      {7, 5, 9, 10}
                                                    };

                /* Inverse Matrix should be */
                /*  25 -41  10 -6 */
                /* -41  68 -17 10 */
                /*  10 -17   5 -3 */
                /*  -6  10  -3  2 */

                A = createMatrix(4);
                fillMatrix(A, matrixToInverse);
            }
            else {
                long double **matrixToInverse;

                int i;
                long double *data = (long double *)calloc(n *n, sizeof(long double));
                matrixToInverse = (long double **)calloc(n, sizeof(long double *));
                for (i = 0; i < n; ++i)
                    matrixToInverse[i] = &(data[n*i]);

                A = createMatrix(n);
                if (my_rank == ROOT_ID) {
                    generateMatrix(n, matrixToInverse);
                    fillMatrixDynamicArray(A, matrixToInverse);
                }
                MPI_Bcast ((A->matrix[0]), n*n, MPI_LONG_DOUBLE, ROOT_ID, MPI_COMM_WORLD);
            }
            //            printMatrixWithName(A, "A");

            struct timeval begin, end;
            if (my_rank == ROOT_ID)
                gettimeofday(&begin, NULL);

            SquareMatrix *A_1 = inverse(A);

            MPI_Barrier(MPI_COMM_WORLD);
            if (my_rank == ROOT_ID) {
                gettimeofday(&end, NULL);
                saveResults(n ? n : 4, begin, end);
            }

            //            if (my_rank == ROOT_ID) {
            //                printMatrix(A_1);
            //            }

            freeMatrix(A);
            freeMatrix(A_1);
        }
    } else {
        ShowUsage ();
    }

    MPI_Finalize();


    return 0;
}

void generateMatrix(int n, long double *matrix[])
{
    int i, j;

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            matrix[i][j] = 1.0 / (i + j + 1.0);
        }
    }
}

void saveResults(int size, struct timeval begin, struct timeval end)
{
    FILE *fp = fopen("output.txt","a+");
    if (fp == NULL) perror ("Error opening file");
    else {
        fprintf(fp,"%d ", size);
        fprintf(fp,"%d ", numprocs);
        fprintf(fp,"%f\n", (double)((end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec) * 0.000001));
        fclose(fp);
    }
}
