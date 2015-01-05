#ifndef LU_DECOMPOSITION_H
#define LU_DECOMPOSITION_H

#include <assert.h>

#include "matrix.h"

/**
 * @brief Returns lower triangular matrix
 *
 * @param mat matrix
 * @return SquareMatrix
 */
SquareMatrix* getL(SquareMatrix *mat);
/**
 * @brief Returns upper triangular matrix
 *
 * @param mat matrix
 * @return SquareMatrix
 */
SquareMatrix* getU(SquareMatrix *mat);
/**
 * @brief Returns inversed lower triangular matrix
 *
 * @param mat_L matrix
 * @return SquareMatrix
 */
SquareMatrix* getLInverse(SquareMatrix *mat_L);
/**
 * @brief Returns inversed upper triangular matrix
 *
 * @param mat_U matrix
 * @return SquareMatrix
 */
SquareMatrix* getUInverse(SquareMatrix *mat_U);

/**
 * @brief LU decomposition based on doolittle algorithm
 *
 * @param mat_A matrix
 * @param mat_LU matrix
 * @return int
 */
int doolittle(SquareMatrix* mat_A, SquareMatrix *mat_LU);
/**
 * @brief LU decomposition based on cholesky algorithm
 *
 * @param mat_A matrix
 * @param mat_LU matrix
 * @return int
 */
int cholesky(SquareMatrix* mat_A, SquareMatrix *mat_LU);
/**
 * @brief LU decomposition based on mod cholesky algorithm
 *
 * @param mat_A matrix
 * @param mat_LU matrix
 * @return int
 */
int choleskyRow(SquareMatrix* mat_A, SquareMatrix *mat_LU);

/**
 * @brief Returns inversed matrix from LU
 *
 * @param mat_LU matrix
 * @return SquareMatrix
 */
SquareMatrix* getInverseMatrixFromLU(SquareMatrix *mat_LU);
/**
 * @brief Inverting a matrix
 *
 * @param mat_A matrix
 * @return SquareMatrix
 */
SquareMatrix* inverse(SquareMatrix *mat_A);

#endif
