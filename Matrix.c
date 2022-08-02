#include "Matrix.h"
#include <stdlib.h>
#include <stdbool.h>

bool createVector(Vector* vector, const size_t size) {
    vector->data = (double*) malloc(sizeof(double) * size);
    bool success = vector->data != NULL;
    if (success) {
        vector->size = size;
    }
    return success;
}

void deleteVector(Vector* vector) {
    if (vector->data != NULL) {
        free(vector->data);
        vector->data = NULL;
        vector->size = 0;
    }
}

void deleteMatrix(Matrix* matrix) {
    for (size_t i = 0; i < matrix->columnSize; i++) {
        Vector* column = &matrix->columns[i];
        if (column->data != NULL) {
            deleteVector(column);
        }
    }

    matrix->columnSize = 0;
    matrix->rowSize = 0;

    free(matrix->columns);
    matrix->columns = NULL;
}

bool createMatrixColumns(Matrix* matrix) {
    for (size_t i = 0; i < matrix->rowSize; i++) {
        Vector* column = &matrix->columns[i];
        if (!createVector(column, matrix->columnSize)) {
            return false; // at least 1 vector creation failed
        }
        for (size_t row = 0; row < column->size; row++) {
            column->data[row] = 0;
        }
    }
    return true; // all vector successfully created
}

bool createMatrix(Matrix* matrix, const size_t rows, const size_t cols) {
    matrix->columns = (Vector*) malloc(sizeof(Vector) * cols);
    bool success = matrix->columns != NULL;
    if (success) {
        matrix->rowSize = cols;
        matrix->columnSize = rows;

        success = createMatrixColumns(matrix);

        if (!success) {
           deleteMatrix(matrix);
        }
    }
    return success;
}

Dimensions2D getDimensions(Matrix* matrix) {
    Dimensions2D dimensions = { 0, 0 };
    if (matrix != NULL) {
        dimensions.rows = matrix->columnSize;
        dimensions.cols = matrix->rowSize;    
    }
    return dimensions;
}

bool isValidCoordinate(const Matrix* matrix, const size_t row, const size_t col) {
    return row < matrix->columnSize && col < matrix->rowSize;
}

double getMatrixElement(const Matrix* matrix, const size_t row, const size_t col) {
    double value = 0;
    if (matrix != NULL && matrix->columns != NULL) {
        if (isValidCoordinate(matrix, row, col)) {
            value = matrix->columns[col].data[row];
        }
    }
    return value;
}

bool setMatrixElement(Matrix* matrix, const size_t row, const size_t col, const double value) {
    bool success = false;
    if (matrix != NULL && matrix->columns != NULL) {
        if (isValidCoordinate(matrix, row, col)) {
            matrix->columns[col].data[row] = value;
            success = true;
        }
    }
    return success;
}