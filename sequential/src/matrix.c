#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy

#include "matrix.h"

void printMatrix(SquareMatrix *mat)
{
    double **m = mat->matrix;
    int s = mat->size;
    int i, j;

    for (i = 0; i < s; ++i) {
        for (j = 0; j < s; ++j) {
            printf("%f ", m[i][j]);
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
    mat->matrix = calloc(size, sizeof(double*));

    double **matrix = mat->matrix;
    int i;

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
    double **m = mat->matrix;
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
    double **m_ret = mat_ret->matrix;
    double **m = mat->matrix;
    int i, j;

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
    for(i = 0; i < mat->size; ++i)
        free(mat->matrix[i]);
    free(mat->matrix);

    free(mat);
}
void fillMatrix(SquareMatrix *dest, double matrix[][dest->size])
{
    int s = dest->size;
    double **mat = dest->matrix;
    int i;

    int sizeOfCell = sizeof(**mat);
    for (i = 0; i < s; ++i) {
        memcpy(mat[i], *(matrix+i), s * sizeOfCell);
    }
}
void copyMatrix(SquareMatrix *dest, SquareMatrix *src)
{
    int s = dest->size;
    double **mat = dest->matrix;
    double **mat_Src = src->matrix;
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
    double **m_1 = mat_1->matrix;
    double **m_2 = mat_2->matrix;
    double **m = mat->matrix;

    int i, j, k;

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
    double **m = mat->matrix;

    int i, j;

    for (i = 0; i < s; ++i) {
        for (j = i + 1; j < s; ++j) {
            if (m[i][j] != m[j][i]) {
                ret = 0;
                goto end;
            }
        }
    }

end:

    return ret;
}
