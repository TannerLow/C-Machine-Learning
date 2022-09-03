#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdbool.h>

// A 1D matrix, 1 col, N rows
typedef struct {
    double* data;
    size_t size;
} Vector;

typedef struct {
    size_t rows;
    size_t cols;
} Dimensions2D;

// matrix[col][row]
typedef struct {
    Vector* columns;
    size_t columnSize; // number of rows
    size_t rowSize; // number of columns
} Matrix;

bool createVector(Vector* vector, const size_t size);
void deleteVector(Vector* vector);

bool createMatrix(Matrix* matrix, const size_t rows, const size_t cols);
bool createMatrixFrom2DArray(Matrix* matrix, const double** arr, const size_t rows, const size_t cols);
void deleteMatrix(Matrix* matrix);
bool clearMatrix(Matrix* matrix);

Dimensions2D getDimensions(const Matrix* matrix);
bool isValidCoordinate(const Matrix* matrix, const size_t row, const size_t col);
bool copyVector(const Vector* a, Vector* b);
bool copyMatrix(const Matrix* a, Matrix* b);

// getter assumption: expected output for invalid get is 1.0e-300 //minimum double value
double getMatrixElement(const Matrix* matrix, const size_t row, const size_t col);
bool setMatrixElement(Matrix* matrix, const size_t row, const size_t col, const double value);
double getVectorElement(const Vector* vector, const size_t i);
bool setVectorElement(Vector* vector, const size_t i, const double value);

bool areEqualSizes(Dimensions2D a, Dimensions2D b);
bool areEqualSizedVectors(const Vector* a, const Vector* b);
bool areEqualVectors(Vector* a, Vector* b);
bool areEqualMatrices(Matrix* a, Matrix* b);

#endif // MATRIX_H