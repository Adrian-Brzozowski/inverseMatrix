#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy

#include <mpi/mpi.h>
#include "utils.h"

#include "matrix.h"

void printMatrix(SquareMatrix *mat)
{
    //    if (my_rank != ROOT_ID) return;

    long double **m = mat->matrix;
    int s = mat->size;
    int i, j;

    for (i = 0; i < s; ++i) {
        for (j = 0; j < s; ++j) {
            printf("%Lf ", m[i][j]);
        }
        printf("\n");
    }
}
void printMatrixWithName(SquareMatrix *mat, char *name)
{
    //    if (my_rank != ROOT_ID) return;

    printf("Matrix %s\n", name);
    printMatrix(mat);
}

SquareMatrix* createMatrix(int size)
{
    SquareMatrix *mat = malloc(sizeof(SquareMatrix));
    mat->size = size;

    long double *data = (long double *)calloc(size * size, sizeof(long double)); // contiguous memory
    if (data == NULL) { perror("Allocation failed"); return mat; }
    mat->matrix = (long double **)calloc(size, sizeof(long double*));
    if (mat->matrix == NULL) { perror("Allocation failed"); return mat; }

    long double **matrix = mat->matrix;
    int i;

    for (i = 0; i < size; ++i)
        matrix[i] =  &(data[size*i]);

    return mat;
}
SquareMatrix* createMatrixFromMatrix(SquareMatrix *src)
{
    SquareMatrix *mat = createMatrix(src->size);
    copyMatrix(mat, src);

    return mat;
}
SquareMatrix* createIdentityMatrix(int size)
{
    SquareMatrix *mat = createMatrix(size);
    long double **m = mat->matrix;
    int i;

    for (i = 0; i < mat->size; ++i) {
        m[i][i] = 1.0;
    }

    return mat;
}
SquareMatrix* transpose(SquareMatrix *mat)
{
    int s = mat->size;
    SquareMatrix *mat_ret = createMatrix(s);
    long double **m_ret = mat_ret->matrix;
    long double **m = mat->matrix;
    int i, j;

    int *iLoopProc = calloc(s, sizeof(int));

    if (my_rank == ROOT_ID) {
        int chunk; // MPI
        int procTest; // MPI
        int iMin, iMax; // MPI
        chunk = s / numprocs;
        procTest = s % numprocs;
        /* divide loop */
        for (i = 0; i < numprocs; ++i) {
            iMin = chunk * i + (procTest < i ? procTest : i);
            iMax = iMin + chunk + (procTest > i);
            for (j = iMin; j < iMax; ++j)
                iLoopProc[j] = i;
        }
    }

    MPI_Bcast (iLoopProc, s, MPI_INT, 0, MPI_COMM_WORLD);

    for(i = 0; i < s; ++i) {
        if (iLoopProc[i] == my_rank) {
            for(j = 0; j < s; ++j) {
                m_ret[i][j] = m[j][i];
            }
        }
        MPI_Bcast (m_ret[i], s, MPI_LONG_DOUBLE, iLoopProc[i], MPI_COMM_WORLD);
    }

    return mat_ret;
}

void freeMatrix(SquareMatrix *mat)
{
    free(mat->matrix[0]);
    free(mat->matrix);
    free(mat);
}
void fillMatrix(SquareMatrix *dest, long double matrix[][dest->size])
{
    int s = dest->size;
    long double **mat = dest->matrix;

    int i;
    int sizeOfCell = sizeof(**mat);
    for (i = 0; i < s; ++i) {
        memcpy(mat[i], *(matrix+i), s * sizeOfCell);
    }
}
void fillMatrixDynamicArray(SquareMatrix *dest, long double *matrix[])
{
    int s = dest->size;
    long double **mat = dest->matrix;

    int i;
    int sizeOfCell = sizeof(**mat);
    for (i = 0; i < s; ++i) {
        memcpy(mat[i], matrix[i], s * sizeOfCell);
    }
}
void copyMatrix(SquareMatrix *dest, SquareMatrix *src)
{
    int s = dest->size;
    long double **mat = dest->matrix;
    long double **mat_Src = src->matrix;
    int i;

    for (i = 0; i < s; ++i) {
        memcpy(mat[i], mat_Src[i], s * sizeof(**mat));
    }
}
SquareMatrix* multiply(SquareMatrix *mat_1, SquareMatrix *mat_2)
{
    assert(mat_1->size == mat_2->size);

    int s = mat_1->size;
    SquareMatrix *mat = createMatrix(s);
    long double **m_1 = mat_1->matrix;
    long double **m_2 = mat_2->matrix;
    long double **m = mat->matrix;

    int i, j, k;

    int *iLoopProc = calloc(s, sizeof(int));

    if (my_rank == ROOT_ID) {
        int chunk; // MPI
        int procTest; // MPI
        int iMin, iMax; // MPI
        chunk = s / numprocs;
        procTest = s % numprocs;
        /* divide loop */
        for (i = 0; i < numprocs; ++i) {
            iMin = chunk * i + (procTest < i ? procTest : i);
            iMax = iMin + chunk + (procTest > i);
            for (j = iMin; j < iMax; ++j)
                iLoopProc[j] = i;
        }
    }

    MPI_Bcast (iLoopProc, s, MPI_INT, 0, MPI_COMM_WORLD);

    /////////////////// BASED ON MPI_Bcast
    for(i = 0; i < s; ++i) {
        if (iLoopProc[i] == my_rank) {
            for(j = 0; j < s; ++j) {
                for(k = 0; k < s; ++k) {
                    m[i][j] += m_1[i][k] * m_2[k][j];
                }
            }
        }

        MPI_Bcast (m[i], s, MPI_LONG_DOUBLE, iLoopProc[i], MPI_COMM_WORLD);
    }

    ///////////////// BASED ON MPI_Send and MPI_Recv
    //    for(i = 0; i < s; ++i) {
    //        if (iLoopProc[i] == my_rank) {
    //            for(j = 0; j < s; ++j) {
    //                for(k = 0; k < s; ++k) {
    //                    m[i][j] += m_1[i][k] * m_2[k][j];
    //                }
    //            }
    //            if (my_rank > ROOT_ID)
    //                MPI_Send(m[i], s, MPI_LONG_DOUBLE, 0, i, MPI_COMM_WORLD);
    //        }

    //        else if (my_rank ==  ROOT_ID && iLoopProc[i] > ROOT_ID)
    //            MPI_Recv(m[i], s, MPI_LONG_DOUBLE, iLoopProc[i], i, MPI_COMM_WORLD, &status);
    //    }

    return mat;
}
int isSymmetric(SquareMatrix *mat)
{
    int ret = 1;
    int s = mat->size;
    long double **m = mat->matrix;

    int i, j;

    for (i = 0; i < s; ++i) {
        for (j = i + 1; j < s; ++j) {
            if (m[i][j] != m[j][i]) {
                ret = 0;
                break;
            }
        }
    }

    return ret;
}
int checkIdentity(SquareMatrix *mat_1, SquareMatrix *mat_2)
{
    int ret = 0;

    int s_1 = mat_1->size;
    int s_2 = mat_2->size;

    if (s_1 > s_2) return -1;
    else if (s_1 < s_2) return 1;

    long double **m_1 = mat_1->matrix;
    long double **m_2 = mat_2->matrix;

    int i, j;

    for (i = 0; i < s_1; ++i) {
        for (j = 0; j < s_1; ++j) {
            if (m_1[i][j] != m_2[i][j]) {
                if (m_1[i][j] > m_2[i][j]) return -1;
                else if (m_1[i][j] < m_2[i][j]) return 1;
            }
        }
    }

    return ret;
}
