#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy

#include <omp.h>

#include "matrix.h"

void printMatrix(SquareMatrix *mat)
{
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
    printf("Matrix %s\n", name);
    printMatrix(mat);
}

SquareMatrix* createMatrix(int size)
{
    SquareMatrix *mat = malloc(sizeof(SquareMatrix));
    mat->size = size;
    mat->matrix = calloc(size, sizeof(long double*));

    long double **matrix = mat->matrix;
    int i;
#pragma omp parallel for private(i) shared(matrix, size)
    for (i = 0; i < size; ++i) {
        matrix[i] = calloc(size, sizeof(**matrix));
    }

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
#pragma omp parallel for private(i) shared(m, mat)
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
#pragma omp parallel for private(i, j) shared(s, m_ret, m)
    for(i = 0; i < s; ++i) {
        for(j = 0; j < s; ++j) {
            m_ret[j][i] = m[i][j];
        }
    }

    return mat_ret;
}

void freeMatrix(SquareMatrix *mat)
{
    int i;
#pragma omp parallel for private(i) shared(mat)
    for(i = 0; i < mat->size; ++i)
        free(mat->matrix[i]);
    free(mat->matrix);

    free(mat);
}
void fillMatrix(SquareMatrix *dest, long double matrix[][dest->size])
{
    int s = dest->size;
    long double **mat = dest->matrix;

    int i;
    int sizeOfCell = sizeof(**mat);
#pragma omp parallel for private(i) shared(mat, matrix, s, sizeOfCell)
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
#pragma omp parallel for private(i) shared(mat, matrix, s, sizeOfCell)
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
    int sizeOfCell = sizeof(**mat);
#pragma omp parallel for private(i) shared(mat, mat_Src, s, sizeOfCell)
    for (i = 0; i < s; ++i) {
        memcpy(mat[i], mat_Src[i], s * sizeof(sizeOfCell));
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
#pragma omp parallel for shared(m,m_1,m_2) private(i,j,k), schedule (static)
    for(i = 0; i < s; ++i) {
        for(j = 0; j < s; ++j) {
            for(k = 0; k < s; ++k) {
                m[i][j] += m_1[i][k] * m_2[k][j];
            }
        }
    }

    return mat;
}
int isSymmetric(SquareMatrix *mat)
{
    int ret = 1;
    int s = mat->size;
    long double **m = mat->matrix;

    int i, j;
#pragma omp parallel for shared(m, ret) private(i,j), schedule (static)
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
