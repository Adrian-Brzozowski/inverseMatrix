#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy

#include "matrix.h"

void printMatrix(SquareMatrix *mat)
{
    double **m = mat->matrix;
    int dim = mat->dimension;
    int i, j;

    for (i = 0; i < dim; ++i) {
        for (j = 0; j < dim; ++j) {
            printf("%7.2f ", m[i][j]);
        }
        printf("\n");
    }
}
void printMatrixWithName(SquareMatrix *mat, char *name)
{
    printf("Matrix %s\n", name);
    printMatrix(mat);
}

SquareMatrix* createMatrix(int dimension)
{
    SquareMatrix *mat = malloc(sizeof(SquareMatrix));
    mat->dimension = dimension;
    mat->matrix = calloc(dimension, sizeof(double*));

    double **matrix = mat->matrix;
    int i;

    for (i = 0; i < dimension; ++i) {
        matrix[i] = calloc(dimension, sizeof(**matrix));
    }

    return mat;
}
SquareMatrix* createMatrixFromMatrix(SquareMatrix *src)
{
    SquareMatrix *mat = createMatrix(src->dimension);
    copyMatrix(mat, src);

    return mat;
}
SquareMatrix* createIdentityMatrix(int dimension)
{
    SquareMatrix *mat = createMatrix(dimension);
    double **m = mat->matrix;
    int i;

    for (i = 0; i < mat->dimension; ++i) {
        m[i][i] = 1.0;
    }

    return mat;
}
SquareMatrix* transpose(SquareMatrix *mat)
{
    int dim = mat->dimension;
    SquareMatrix *mat_ret = createMatrix(dim);
    double **m_ret = mat_ret->matrix;
    double **m = mat->matrix;
    int i, j;

    for(i = 0; i < dim; ++i) {
        for(j = 0; j < dim; ++j) {
            m_ret[j][i] = m[i][j];
        }
    }

    return mat_ret;
}

void freeMatrix(SquareMatrix *mat)
{
    int i;
    for(i = 0; i < mat->dimension; ++i)
        free(mat->matrix[i]);
    free(mat->matrix);

    free(mat);
}

void fillMatrix(SquareMatrix *dest, double matrix[][dest->dimension])
{
    int dim = dest->dimension;
    double **mat = dest->matrix;
    int i;

    int sizeOfCell = sizeof(**mat);
    for (i = 0; i < dim; ++i) {
        memcpy(mat[i], *(matrix+i), dim * sizeOfCell);
    }
}

void copyMatrix(SquareMatrix *dest, SquareMatrix *src)
{
    int dim = dest->dimension;
    double **mat = dest->matrix;
    double **mat_Src = src->matrix;
    int i;

    for (i = 0; i < dim; ++i) {
        memcpy(mat[i], mat_Src[i], dim * sizeof(**mat));
    }
}

SquareMatrix* multiply(SquareMatrix *mat_1, SquareMatrix *mat_2)
{
    assert(mat_1->dimension == mat_2->dimension);

    int dim = mat_1->dimension;
    SquareMatrix *mat = createMatrix(dim);
    double **m_1 = mat_1->matrix;
    double **m_2 = mat_2->matrix;
    double **m = mat->matrix;

    int i, j, k;

    for(i = 0; i < dim; ++i) {
        for(j = 0; j < dim; ++j) {
            for(k = 0; k < dim; ++k) {
                m[i][j] += m_1[i][k] * m_2[k][j];
            }
        }
    }

    return mat;
}

int isSymmetric(SquareMatrix *mat)
{
    int ret = 1;
    int dim = mat->dimension;
    double **m = mat->matrix;

    int i, j;

    for (i = 0; i < dim; ++i) {
        for (j = i + 1; j < dim; ++j) {
            if (m[i][j] != m[j][i]) {
                ret = 0;
                goto end;
            }
        }
    }

end:

    return ret;
}
