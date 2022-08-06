#include "Matrix.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

bool createVector(Vector* vector, const size_t size) {
    assert(vector != NULL);

    if (vector == NULL) return false;

    vector->data = (double*) malloc(sizeof(double) * size);
    bool success = vector->data != NULL;
    if (success) {
        vector->size = size;
    }

    return success;
}

void deleteVector(Vector* vector) {
    assert(vector != NULL);

    if (vector->data != NULL) {
        free(vector->data);
        vector->data = NULL;
        vector->size = 0;
    }
}

void deleteMatrix(Matrix* matrix) {
    assert(matrix != NULL);

    if (matrix != NULL) {
        for (size_t i = 0; i < matrix->rowSize; i++) {
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
}

bool createMatrixColumns(Matrix* matrix) {
    assert(matrix != NULL);

    bool success = false;
    if (matrix != NULL) {
        for (size_t i = 0; i < matrix->rowSize; i++) {
            Vector* column = &matrix->columns[i];
            if (!createVector(column, matrix->columnSize)) {
                return false; // at least 1 vector creation failed
            }
            for (size_t row = 0; row < column->size; row++) {
                column->data[row] = 0;
            }
        }
        success = true; // all vector successfully created
    }

    return success;
}

bool createMatrix(Matrix* matrix, const size_t rows, const size_t cols) {
    assert(matrix != NULL);

    if (matrix == NULL) return false; // cant set columns for a nullptr

    matrix->columns = (Vector*) malloc(cols * sizeof(Vector));
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
    assert(matrix != NULL);

    Dimensions2D dimensions = { 0, 0 };
    if (matrix != NULL) {
        dimensions.rows = matrix->columnSize;
        dimensions.cols = matrix->rowSize;    
    }
    return dimensions;
}

bool isValidCoordinate(const Matrix* matrix, const size_t row, const size_t col) {
    assert(matrix != NULL);
    return matrix != NULL && row < matrix->columnSize && col < matrix->rowSize;
}

double getMatrixElement(const Matrix* matrix, const size_t row, const size_t col) {
    assert(matrix != NULL);
    double value = 0;
    if (matrix != NULL && matrix->columns != NULL) {
        if (isValidCoordinate(matrix, row, col)) {
            value = matrix->columns[col].data[row];
        }
    }
    return value;
}

bool setMatrixElement(Matrix* matrix, const size_t row, const size_t col, const double value) {
    assert(matrix != NULL);
    bool success = false;
    if (matrix != NULL && matrix->columns != NULL) {
        if (isValidCoordinate(matrix, row, col)) {
            matrix->columns[col].data[row] = value;
            success = true;
        }
    }
    return success;
}

bool areEqualSizes(Dimensions2D a, Dimensions2D b) {
    return a.rows == b.rows && a.cols == b.cols;
}

bool areEqualVectors(Vector* a, Vector* b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;

    bool equal = false;
    if (a->size == b->size) {
        for (size_t i = 0; i < a->size; i++) {
            if (a->data[i] != b->data[i]) {
                return false;
            }
        }
        equal = true;
    }
    return equal;
}

bool areEqualMatrices(Matrix* a, Matrix* b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;

    bool equal = false;
    if (areEqualSizes(getDimensions(a), getDimensions(b))) {
        for (size_t i = 0; i < a->rowSize; i++) {
            if (!areEqualVectors(&a->columns[i], &b->columns[i])) {
                return false;
            }
        }
        equal = true;
    }
    return equal;
}