#include "Matrix.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <float.h>
#include "../stdwrapper.h"

bool createVector(Vector* vector, const size_t size) {
    assert(vector != NULL);

    if (vector == NULL) return false;

    vector->data = (double*) safeMalloc(sizeof(double) * size);
    bool success = vector->data != NULL;
    if (success) {
        vector->size = size;
    }

    return true;
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

bool clearMatrix(Matrix* matrix) {
    assert(matrix != NULL);
    assert(matrix->columns != NULL);

    for (size_t col = 0; col < matrix->rowSize; col++) {
        for (size_t row = 0; row < matrix->columnSize; row++) {
            assert(setMatrixElement(matrix, row, col, 0));
        }
    }

    return true;
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

bool createMatrixFrom2DArray(Matrix* matrix, const double** arr, const size_t rows, const size_t cols) {
    assert(matrix != NULL);
    assert(arr != NULL);
    
    if (matrix == NULL) return false;
    if (arr == NULL) return false;

    if (!createMatrix(matrix, rows, cols)) return false; // unable to create matrix to store values

    for (size_t col = 0; col < cols; col++) {
        for (size_t row = 0; row < rows; row++) {
            if (!setMatrixElement(matrix, row, col, arr[row][col])) {
                deleteMatrix(matrix);
                return false;
            }
        }
    }

    return true;
}

Dimensions2D getDimensions(const Matrix* matrix) {
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

    double value = 1.0e-300;//DBL_MIN;
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
        //else {
        //    printf("%llu, %llu, %lf\n", row, col, value);
        //}
    }
    return success;
}

double getVectorElement(const Vector* vector, const size_t i) {
    assert(vector != NULL);
    assert(i < vector->size);

    double value = DBL_MIN;
    if (vector != NULL && i < vector->size) {
        value = vector->data[i];
    }

    return value;
}

bool setVectorElement(Vector* vector, const size_t i, const double value) {
    assert(vector != NULL);
    assert(i < vector->size);

    if (vector != NULL && i < vector->size) {
        vector->data[i] = value;
    }
    
    return value;
}

bool copyVector(const Vector* a, Vector* b) {
    assert(a != NULL && b != NULL);
    assert(a->data != NULL && b->data != NULL);
    assert(a->size == b->size);

    // input validation
    if (a == NULL || b == NULL) return false;
    if (a->data == NULL || b->data == NULL) return false;
    if (a->size != b->size) return false;

    // actual work
    for (size_t i = 0; i < a->size; i++) {
        b->data[i] = a->data[i];
    }

    return true;
}

bool copyMatrix(const Matrix* a, Matrix* b) {
    assert(a != NULL && b != NULL);
    assert(a->columns != NULL && b->columns != NULL);
    assert(a->columns->data != NULL && b->columns->data != NULL);
    assert(areEqualSizes(getDimensions(a), getDimensions(b)));

    // input validation
    if (a == NULL || b == NULL) return false;
    if (a->columns == NULL || b->columns == NULL) return false;
    if (a->columns->data == NULL || b->columns->data == NULL) return false;
    if (!areEqualSizes(getDimensions(a), getDimensions(b))) return false;

    // actual work
    for (size_t col = 0; col < a->rowSize; col++) {
        for (size_t row = 0; row < a->columnSize; row++) {
            setMatrixElement(b, row, col, getMatrixElement(a, row, col));
        }
    }

    return true;
}

bool areEqualSizes(Dimensions2D a, Dimensions2D b) {
    return a.rows == b.rows && a.cols == b.cols;
}

bool areEqualSizedVectors(const Vector* a, const Vector* b) {
    assert(a != NULL && b != NULL);
    
    // input validation
    if (a == NULL || b == NULL) return false;

    return a->size == b->size;
}

bool areEqualVectors(Vector* a, Vector* b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;

    bool equal = false;
    if (a->size == b->size) {
        equal = true;
        for (size_t i = 0; i < a->size; i++) {
            if (a->data[i] != b->data[i]) {
                equal = false;
            }
        }
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