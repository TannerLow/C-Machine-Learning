#include "tester.h"
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "MatrixMath.h"

typedef struct {
    Matrix* matrices;
    size_t size;
} MatrixArray;

void logTestError(const char* msg, int line);
void displayMatrix(Matrix* m);
bool initializeMatrixArray(MatrixArray* array, const size_t size);
void freeMatrixArray(MatrixArray* array);
bool initializeMatrixSet(MatrixArray** set, const size_t size);
void freeMatrixArraySet(MatrixArray** set, const size_t size);
bool loadMatricesFromFile(const char* filename, MatrixArray** array, size_t* numOfSets);
bool dotProductTest();

void test() {
    printf("Dot Product Test: %s\n", dotProductTest() ? "sucess" : "FAILURE");
    printf("Done Testing\n");
}

void logTestError(const char* msg, int line) {
    printf("%s (%s, %d)\n", msg, __FILE__, line);
}

void displayMatrix(Matrix* m) {
    for (size_t row = 0; row < m->columnSize; row++) {
        for (size_t col = 0; col < m->rowSize; col++) {
            printf("%lf ", getMatrixElement(m, row, col));
        }
        printf("\n");
    }
}

// pointer to MatrixArray
bool initializeMatrixArray(MatrixArray* array, const size_t size) {
    if (array == NULL) return false; // can't populate something that doesn't exist

    array->matrices = (Matrix*) malloc(size * sizeof(Matrix));

    if (array->matrices == NULL) {
        return false; // unable to allocate space for incoming matrices
    }

    array->size = size;

    return true;
}

// pointer to MatrixArray
void freeMatrixArray(MatrixArray* array) {
    if (array != NULL) {
        if (array->matrices != NULL) {
            for (size_t i = 0; i < array->size; i++) {
                deleteMatrix(&array->matrices[i]);
            }
            free(array->matrices);
        }
        free(array);
    }
}

// Does nto initialize MatrixArray's, just zeroes them out to be set later
// pointer to MatrixArray[]
bool initializeMatrixSet(MatrixArray** set, const size_t size) {
    if (set == NULL) return false; // can't populate something that doesn't exist

    *set = (MatrixArray*) malloc(size * sizeof(MatrixArray));

    if (*set == NULL) {
        return false; // unable to allocate space for incoming matrices
    }

    MatrixArray emptyArray = { NULL, 0 };
    for (size_t i = 0; i < size; i++) {
        *set[i] = emptyArray;
    }

    return true;
}

// pointer to MatrixArray[]
void freeMatrixArraySet(MatrixArray** set, const size_t size) {
    if (set != NULL && *set != NULL) {
        for (size_t i = 0; i < size; i++) {
            freeMatrixArray(&(*set[i]));
        }
        free(*set);
    }
}

bool loadMatricesFromFile(const char* filename, MatrixArray** array, size_t* numOfArrays) {
    FILE* filePtr = fopen(filename, "r");
    
    if (filePtr == NULL) {
        return false; // unable to open file
    }

    // create array of MatrixArray's
    fscanf(filePtr, "%llu", numOfArrays);
    if (!initializeMatrixSet(array, *numOfArrays)) {
        return false; // failed to allocate space for MatrixArraySet/incoming matrices
    }

    // for each MatrtixArray in the set of MatrixArray's
    for (size_t i = 0; i < *numOfArrays; i++) {
        // get desired size of MatrixArray
        size_t n;
        fscanf(filePtr, "%llu", &n);

        // allocate memory for the matrices in the MatrixArray
        MatrixArray* matrixArray = (*array) + i;
        if (!initializeMatrixArray(matrixArray, n)) {
            freeMatrixArraySet(array, *numOfArrays);
            return false; // error when initializing a MatrixArray in the MatrixArraySet
        }

        for (size_t j = 0; j < matrixArray->size; j++) {
            size_t rows, cols;
            fscanf(filePtr, "%llu %llu", &rows, &cols);

            Matrix* matrix = &(matrixArray->matrices[j]);

            if (!createMatrix(matrix, rows, cols)) {
                logTestError("Failed to create matrix", __LINE__);
                freeMatrixArraySet(array, *numOfArrays);
                return false; // Error with call to createMatrix
            }

            for (int row = 0; row < rows; row++) {
                for(int col = 0; col < cols; col++) {
                    double value;
                    fscanf(filePtr, "%lf", &value);
                    if (!setMatrixElement(matrix, row, col, value)) {
                        freeMatrixArraySet(array, *numOfArrays);
                        return false;
                    }

                }
            }

            printf("Rows: %d, Cols: %d\n", rows, cols);
            displayMatrix(matrix);
        }
    }

    fclose(filePtr);
    return true;
}

bool dotProductTest() {
    MatrixArray* matrixArrays;
    size_t size;

    if (!loadMatricesFromFile("tests/dot_product.test", &matrixArrays, &size)) {
        printf("Failed to load test data from file: tester.c,%d\n", __LINE__);
        return false;
    }

    printf("Beginning dot product testing: %d test case(s)\n", size);

    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* b = &matrixArrays[testCase].matrices[1];
        Matrix* expected = &matrixArrays[testCase].matrices[2];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            freeMatrixArraySet(&matrixArrays, size);
            return false;
        }

        if(!dotProduct(a, b, &actual)) {
            logTestError("Failed to perform dot product", __LINE__);
            freeMatrixArraySet(&matrixArrays, size);
            return false;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Matrix B:\n");
        displayMatrix(b);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        if (!success) {
            return false;
        }
    }

    return true;
}