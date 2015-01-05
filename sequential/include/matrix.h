#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>

/**
 * @brief Structure containing matrix
 *
 */
typedef struct {
    int dimension; /**< dimension */
    double **matrix; /**< data of matrix */
} SquareMatrix;

/**
 * @brief Creates a matrix of a given size
 *
 * @param dimension dimension
 * @return SquareMatrix
 */
SquareMatrix* createMatrix(int dimension);
/**
 * @brief Creates a matrix based on another
 *
 * @param src source matrix
 * @return SquareMatrix
 */
SquareMatrix* createMatrixFromMatrix(SquareMatrix *src);
/**
 * @brief Creates an identity matrix
 *
 * @param dimension dimension
 * @return SquareMatrix
 */
SquareMatrix* createIdentityMatrix(int dimension);

/**
 * @brief Releases matrix
 *
 * @param mat matrix
 */
void freeMatrix(SquareMatrix *mat);

/**
 * @brief Fills matrix using an array
 *
 * @param dest destination matrix
 * @param matrix[][]
 */
void fillMatrix(SquareMatrix *dest, double matrix[][dest->dimension]);
/**
 * @brief Copies matrix
 *
 * @param dest destination matrix
 * @param src source matrix
 */
void copyMatrix(SquareMatrix *dest, SquareMatrix *src);

/**
 * @brief Print matrix
 *
 * @param mat matrix
 */
void printMatrix(SquareMatrix *mat);
/**
 * @brief Print matrix with name
 *
 * @param mat matrix
 * @param name name
 */
void printMatrixWithName(SquareMatrix *mat, char* name);

/**
 * @brief Checks if the matrix is symmetric
 *
 * @param mat matrix
 * @return int
 */
int isSymmetric(SquareMatrix *mat);

/**
 * @brief Transpose matrix
 *
 * @param mat matrix
 * @return SquareMatrix
 */
SquareMatrix* transpose(SquareMatrix *mat);
/**
 * @brief Multiply matrices
 *
 * @param mat_1 matrix
 * @param mat_2 matrix
 * @return SquareMatrix
 */
SquareMatrix* multiply(SquareMatrix *mat_1, SquareMatrix *mat_2);

#endif

