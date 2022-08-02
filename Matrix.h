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

bool createMatrix(Matrix* m, const size_t rows, const size_t cols);
void deleteMatrix(Matrix* m);

Dimensions2D getDimensions(Matrix* matrix);
bool isValidCoordinate(const Matrix* matrix, const size_t row, const size_t col);
double getMatrixElement(const Matrix* matrix, const size_t row, const size_t col);
bool setMatrixElement(Matrix* matrix, const size_t row, const size_t col, const double value);

#endif // MATRIX_H