#include "lu.h"

#include <math.h>
#include <stdio.h>

#include "utils.h"

#include <mpi/mpi.h>

#define DEBUG_VERBOSE_INVERSE 0
#define DEBUG_LU_DECOMPOSITION_TEST 0

SquareMatrix* getL(SquareMatrix *mat)
{
    int s = mat->size;
    SquareMatrix *mat_ret = createIdentityMatrix(s);
    long double **m_src = mat->matrix;
    long double **m_dest = mat_ret->matrix;
    int i, j;

    int *iLoopProc = calloc(s, sizeof(int));

    if (my_rank == ROOT_ID)
        divideLoop(s-1, iLoopProc);
    MPI_Bcast(iLoopProc, s, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 1; i < s; ++i) {
        if (iLoopProc[i] == my_rank) {
            for (j = 0; j < i; ++j) {
                m_dest[i][j] = m_src[i][j];
            }
        }
        MPI_Bcast (m_dest[i], s, MPI_LONG_DOUBLE, iLoopProc[i], MPI_COMM_WORLD);
    }

    return mat_ret;
}

SquareMatrix* getU(SquareMatrix *mat)
{
    int s = mat->size;
    SquareMatrix *mat_ret = createMatrix(s);
    long double **m_src = mat->matrix;
    long double **m_dest = mat_ret->matrix;

    int i, j;

    int *iLoopProc = calloc(s, sizeof(int));

    if (my_rank == ROOT_ID)
        divideLoop(s-1, iLoopProc);
    MPI_Bcast(iLoopProc, s, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < s; ++i) {
        if (iLoopProc[i] == my_rank) {
            for (j = i; j < s; ++j) {
                m_dest[i][j] = m_src[i][j];
            }
        }
        MPI_Bcast (m_dest[i], s, MPI_LONG_DOUBLE, iLoopProc[i], MPI_COMM_WORLD);
    }

    return mat_ret;
}

SquareMatrix* getLInverse(SquareMatrix *mat_L)
{
    int s = mat_L->size;
    SquareMatrix *mat_ret = createMatrix(s);
    SquareMatrix *mat_I = createIdentityMatrix(s);

    long double **m_L = mat_L->matrix;
    long double **m_I = mat_I->matrix;
    long double **m = mat_ret->matrix;

    int i, k, j;
    long double sum;

    long double mySum; // MPI
    int chunk; // MPI
    int procTest; // MPI
    int jMin, jMax; // MPI

    for(k = 0; k < s; ++k)
    {
        for(i = k; i < s; ++i)
        {
//            mySum = 0.0;
//            chunk = (i-k) / numprocs;
//            procTest = (i-k) % numprocs;
//            /* divide loop */
//            jMin = k + chunk * my_rank + (procTest < my_rank ? procTest : my_rank);
//            jMax = jMin + chunk + (procTest > my_rank);

//            for(j = jMin; j < jMax; ++j) mySum +=  m_L[i][j] * m[j][k];
//            MPI_Allreduce(&mySum, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

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

    long double **m_U = mat_U->matrix;
    long double **m_I = mat_I->matrix;
    long double **m = mat_ret->matrix;

    int i, j, k;
    long double sum;

    long double mySum; // MPI
    int chunk; // MPI
    int procTest; // MPI
    /* divide loop */
    int jMin, jMax; // MPI

    for(k = 0; k < s; ++k)
    {
        for(i = k; i >= 0; --i)
        {
//            mySum = 0.0;
//            chunk = (k-i) / numprocs;
//            procTest = (k-i) % numprocs;
//            /* divide loop */
//            jMin = i+1 +chunk * my_rank + (procTest < my_rank ? procTest : my_rank);
//            jMax = jMin + chunk + (procTest > my_rank);

//            for(j =jMin; j < jMax ; ++j) mySum +=  m_U[i][j] * m[j][k];
//            MPI_Allreduce (&mySum, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

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

    long double **matrixA = mat_A->matrix;
    long double **matrixLU = mat_LU->matrix;

    int ret = 0;

    int i, j, k;
    long double sum;

    int s = mat_A->size;

    long double mySum; // MPI
    int chunk; // MPI
    int procTest; // MPI
    /* divide loop */
    int kMin, kMax; // MPI

    for(j = 0; j < s; ++j)
    {
        for(i = 0; i <= j; ++i)
        {
//            mySum = 0.0;
//            chunk = i / numprocs;
//            procTest = i % numprocs;
//            /* divide loop */
//            kMin = chunk * my_rank + (procTest < my_rank ? procTest : my_rank);
//            kMax = kMin + chunk + (procTest > my_rank);

//            for(k = kMin; k < kMax; ++k) mySum +=  matrixLU[i][k] * matrixLU[k][j];
//            MPI_Allreduce (&mySum, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

                        sum = 0.0;
                        for(k = 0; k < i; ++k) sum +=  matrixLU[i][k] * matrixLU[k][j];
            matrixLU[i][j] = matrixA[i][j] - sum;
        }
        for(i = j + 1; i < s; ++i)
        {
//            mySum = 0.0;
//            chunk = j / numprocs;
//            procTest = j % numprocs;
//            /* divide loop */
//            kMin = chunk * my_rank + (procTest < my_rank ? procTest : my_rank);
//            kMax = kMin + chunk + (procTest > my_rank);

//            for(k = kMin; k < kMax; ++k) mySum += matrixLU[i][k] * matrixLU[k][j];
//            MPI_Allreduce (&mySum, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
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

    long double **matrixA = A->matrix;
    long double **matrixLU = mat_LU->matrix;

    int ret = 0;

    int i, j, k;
    long double sum;

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

    long double **matrixA = A->matrix;
    long double **matrixLU = mat_LU->matrix;

    int ret = 0;

    int i, j, k;
    long double sum;

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
    long double **m_LU = mat_LU->matrix;
    long double **m_ret = mat_ret->matrix;
    long double **m_I = mat_I->matrix;

    int i, j, k;

    long double sum, y[s];

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

    if(0 && isSymmetric(mat_A)) {
        cholesky(mat_A, LU);

        SquareMatrix *L_1 = getLInverse(LU);
        SquareMatrix *L_1_T = transpose(L_1);

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
#if DEBUG_LU_DECOMPOSITION_TEST
        SquareMatrix *A_ = multiply(L, U);
        //        printMatrixWithName(mat_A, "A");
        //        printMatrixWithName(A_, "A'");

        int cI = checkIdentity(mat_A, A_);
        printf("%s\n", cI == 0 ? "A == A'" : (cI > 1 ? "A < A'" : "A < A'"));

        freeMatrix(A_);
#endif

        MPI_Barrier(MPI_COMM_WORLD);

        SquareMatrix *L_1 = getLInverse(L);
        SquareMatrix *U_1 = getUInverse(U);

        mat_ret = multiply(U_1, L_1);

        if (my_rank == ROOT_ID) {
//            printMatrixWithName(LU, "LU cholesky");
//            printMatrixWithName(L, "L");
//            printMatrixWithName(U, "U");
//            printMatrixWithName(L_1, "L^(-1)");
//            printMatrixWithName(U_1, "U^(-1)");
//            printMatrixWithName(mat_ret, "A^(-1)");
        }

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
