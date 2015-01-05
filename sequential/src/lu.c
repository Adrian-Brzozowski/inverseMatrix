#include "lu.h"

#include <math.h>

#define DEBUG_VERBOSE_INVERSE 0

SquareMatrix* getL(SquareMatrix *mat)
{
    int s = mat->size;
    SquareMatrix *mat_ret = createIdentityMatrix(s);
    double **m_src = mat->matrix;
    double **m_dest = mat_ret->matrix;
    int i, j;

    for (i = 1; i < s; ++i) {
        for (j = 0; j < i; ++j) {
            m_dest[i][j] = m_src[i][j];
        }
    }

    return mat_ret;
}
SquareMatrix* getU(SquareMatrix *mat)
{
    int s = mat->size;
    SquareMatrix *mat_ret = createMatrix(s);
    double **m_src = mat->matrix;
    double **m_dest = mat_ret->matrix;

    int i, j;

    for (i = 0; i < s; ++i) {
        for (j = i; j < s; ++j) {
            m_dest[i][j] = m_src[i][j];
        }
    }

    return mat_ret;
}

SquareMatrix* getLInverse(SquareMatrix *mat_L)
{
    int s = mat_L->size;
    SquareMatrix *mat_ret = createMatrix(s);
    SquareMatrix *mat_I = createIdentityMatrix(s);

    double **m_L = mat_L->matrix;
    double **m_I = mat_I->matrix;
    double **m = mat_ret->matrix;

    int i, k, j;
    double sum;

    for(k = 0; k < s; ++k)
    {
        for(i = k; i < s; ++i)
        {
            sum = 0.0;
            for(j = k; j < i; ++j) sum +=  m_L[i][j] * m[j][k];
            m[i][k] = (m_I[i][k] - sum) / m_L[i][i];
        }
    }

    freeMatrix(mat_I);

    return mat_ret;
}
SquareMatrix* getUInverse(SquareMatrix *mat_U)
{
    int s = mat_U->size;
    SquareMatrix *mat_ret = createMatrix(s);
    SquareMatrix *mat_I = createIdentityMatrix(s);

    double **m_U = mat_U->matrix;
    double **m_I = mat_I->matrix;
    double **m = mat_ret->matrix;

    int i, j, k;
    double sum;

    for(k = 0; k < s; ++k)
    {
        for(i = k; i >= 0; --i)
        {
            sum = 0.0;
            for(j = i+1; j <= k; ++j) sum +=  m_U[i][j] * m[j][k];
            m[i][k] = (m_I[i][k] - sum) / m_U[i][i];
        }
    }

    freeMatrix(mat_I);

    return mat_ret;
}

int doolittle(SquareMatrix *mat_A, SquareMatrix *mat_LU)
{
    assert(mat_A->size == mat_LU->size);

    double **matrixA = mat_A->matrix;
    double **matrixLU = mat_LU->matrix;

    int ret = 0;

    int i, j, k;
    double sum;

    int s = mat_A->size;

    for(j = 0; j < s; ++j)
    {
        for(i = 0; i <= j; ++i)
        {
            sum = 0.0;
            for(k = 0; k < i; ++k) sum +=  matrixLU[i][k] * matrixLU[k][j];
            matrixLU[i][j] = matrixA[i][j] - sum;
        }
        for(i = j + 1; i < s; ++i)
        {
            sum = 0.0;
            for(k = 0; k < j; ++k) sum += matrixLU[i][k] * matrixLU[k][j];
            matrixLU[i][j] = (matrixA[i][j] - sum) / matrixLU[j][j];
        }
    }

    return ret;
}
int cholesky(SquareMatrix *A, SquareMatrix *mat_LU)
{
    assert(A->size == mat_LU->size);

    double **matrixA = A->matrix;
    double **matrixLU = mat_LU->matrix;

    int ret = 0;

    int i, j, k;
    double sum;

    int s = A->size;

    for(k = 0; k < s; ++k){
        sum = 0.0;
        for(j = 0; j < k; ++j) sum += matrixLU[k][j] * matrixLU[k][j];
        matrixLU[k][k] = sqrt(matrixA[k][k] - sum);
        for(i = k+1; i < s; ++i){
            sum = 0.0;
            for(j = 0; j < k; ++j) sum+= matrixLU[i][j] * matrixLU[k][j];
            assert(matrixLU[k][k] != 0.0);
            matrixLU[i][k] = (matrixA[i][k] - sum) / matrixLU[k][k];
        }
    }

    return ret;
}
int choleskyRow(SquareMatrix *A, SquareMatrix *mat_LU)
{
    assert(A->size == mat_LU->size);

    double **matrixA = A->matrix;
    double **matrixLU = mat_LU->matrix;

    int ret = 0;

    int i, j, k;
    double sum;

    int s = A->size;

    for(i = 0; i < s; ++i){
        for(j = 0; j < s; ++j){
            sum = 0.0;
            for(k = 0; k < j; ++k) sum += matrixLU[i][k] * matrixLU[j][k];
            matrixLU[i][j] = (matrixA[i][j] - sum) / matrixLU[j][j];
        }
        sum = 0.0;
        for(k = 0; k < i; ++k) sum += matrixLU[i][k] * matrixLU[i][k];
        matrixLU[i][i] = sqrt(matrixA[i][i] - sum);
    }

    return ret;
}

SquareMatrix* getInverseMatrixFromLU(SquareMatrix *mat_LU)
{
    int s = mat_LU->size;
    SquareMatrix *mat_ret = createMatrix(s);
    SquareMatrix *mat_I = createIdentityMatrix(s);
    double **m_LU = mat_LU->matrix;
    double **m_ret = mat_ret->matrix;
    double **m_I = mat_I->matrix;

    int i, j, k;

    double sum, y[s];

    for(k = 0; k < s; k++) {
        for(i = 0; i < s; i++) {
            sum = 0.0;
            for(j = 0; j <= i-1; j++) {
                sum += m_LU[i][j] * y[j];
            }
            y[i] = (m_I[i][k] - sum);
        }
        for(i = s-1; i >= 0; i--) {
            sum = 0.0;
            for(j = i+1; j < s; j++) {
                sum += m_LU[i][j] * m_ret[j][k];
            }
            m_ret[i][k] = (y[i] - sum) / m_LU[i][i];
        }
    }

    freeMatrix(mat_I);

    return mat_ret;
}

SquareMatrix* inverse(SquareMatrix *mat_A)
{
    int s = mat_A->size;
    SquareMatrix *LU = createMatrix(s);
    SquareMatrix *mat_ret;

    if(isSymmetric(mat_A)) {
        cholesky(mat_A, LU);

        SquareMatrix *L_1 =  getLInverse(LU);
        SquareMatrix *L_1_T =  transpose(L_1);

        mat_ret =  multiply(L_1_T, L_1);

#if DEBUG_VERBOSE_INVERSE
        printMatrixWithName(LU, "LU cholesky");
        printMatrixWithName(L_1, "L^(-1)");
        printMatrixWithName(L_1_T, "(L^(-1))^T");
#endif

        freeMatrix(L_1);
        freeMatrix(L_1_T);
    }
    else {
        doolittle(mat_A, LU);

        SquareMatrix *L = getL(LU);
        SquareMatrix *U = getU(LU);
        SquareMatrix *L_1 = getLInverse(L);
        SquareMatrix *U_1 = getUInverse(U);

        mat_ret = multiply(U_1, L_1);

#if DEBUG_VERBOSE_INVERSE
        printMatrixWithName(LU, "LU doolitle");
        printMatrixWithName(L, "L");
        printMatrixWithName(U, "U");
        printMatrixWithName(L_1, "L^(-1)");
        printMatrixWithName(U_1, "U^(-1)");
#endif

        freeMatrix(L);
        freeMatrix(U);
        freeMatrix(L_1);
        freeMatrix(U_1);
    }

#if DEBUG_VERBOSE_INVERSE
    printMatrixWithName(mat_ret, "A^(-1)");
#endif
    freeMatrix(LU);

    return mat_ret;
}
