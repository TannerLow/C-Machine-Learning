#include "tester.h"
#include <stdio.h>
#include <stdlib.h>
#include "../matrix/Matrix.h"
#include "../matrix/MatrixMath.h"
#include "../matrix/MatrixDebug.h"

typedef struct {
    Matrix* matrices;
    size_t size;
} MatrixArray;

void logTestError(const char* msg, int line);
bool initializeMatrixArray(MatrixArray* array, const size_t size);
void freeMatrixArray(MatrixArray* array);
bool initializeMatrixSet(MatrixArray** set, const size_t size);
void freeMatrixArraySet(MatrixArray** set, const size_t size);
bool loadMatricesFromFile(const char* filename, MatrixArray** array, size_t* numOfSets);
bool dotProductTest();
bool matrixAdditionTest();
bool matrixScaleTest();
bool matrixElementWiseMultiplicationTest();
bool matrixSubtractionTest();
bool dotProductTransposeTest();

void test() {
    bool dotProduct = dotProductTest();
    bool addition = matrixAdditionTest(); 
    bool scalar = matrixScaleTest();  
    bool elementWise = matrixElementWiseMultiplicationTest();
    bool subtraction = matrixSubtractionTest();
    bool dotTranspose = dotProductTransposeTest();

    printf("\n");
    printf("Dot Product Test: %s\n", dotProduct ? "success" : "FAILURE");
    printf("Matrix Addition Test: %s\n", addition ? "success" : "FAILURE");
    printf("Matrix Scalar Test: %s\n", scalar ? "success" : "FAILURE");
    printf("Matrix Element Wise Multiplication Test: %s\n", elementWise ? "success" : "FAILURE");
    printf("Matrix Subtraction Test: %s\n", subtraction ? "success" : "FAILURE");
    printf("Dot Product Transpose B Test: %s\n", dotTranspose ? "success" : "FAILURE");

    printf("Done Testing\n");
}

void logTestError(const char* msg, int line) {
    printf("%s (%s, %d)\n", msg, __FILE__, line);
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
            array->matrices = NULL;
        }
        //free(array);
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
        *set = NULL;
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

    if (!loadMatricesFromFile("tests/tests_data/dot_product.test", &matrixArrays, &size)) {
        logTestError("Failed to load test data from file", __LINE__);
        return false;
    }

    printf("Beginning dot product testing: %d test case(s)\n", size);

    bool testSuccess = true;
    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* b = &matrixArrays[testCase].matrices[1];
        Matrix* expected = &matrixArrays[testCase].matrices[2];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            testSuccess = false;
            break;
        }

        if(!dotProduct(a, b, &actual)) {
            logTestError("Failed to perform dot product", __LINE__);
            deleteMatrix(&actual);
            testSuccess = false;
            break;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Matrix B:\n");
        displayMatrix(b);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        deleteMatrix(&actual);

        if (!success) {
            testSuccess = false;
            break;
        }
    }

    freeMatrixArraySet(&matrixArrays, size);
    return testSuccess;
}

bool matrixAdditionTest() {
    MatrixArray* matrixArrays;
    size_t size;

    if (!loadMatricesFromFile("tests/tests_data/matrix_addition.test", &matrixArrays, &size)) {
        logTestError("Failed to load test data from file", __LINE__);
        return false;
    }

    printf("Beginning matrix addition testing: %d test case(s)\n", size);

    bool testSuccess = true;
    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* b = &matrixArrays[testCase].matrices[1];
        Matrix* expected = &matrixArrays[testCase].matrices[2];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            testSuccess = false;
            break;
        }

        if(!matrixAddition(a, b, &actual)) {
            logTestError("Failed to perform matrix addition", __LINE__);
            deleteMatrix(&actual);
            testSuccess = false;
            break;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Matrix B:\n");
        displayMatrix(b);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        deleteMatrix(&actual);

        if (!success) {
            testSuccess = false;
            break;
        }
    }

    freeMatrixArraySet(&matrixArrays, size);
    return testSuccess;
}

bool matrixScaleTest() {
    MatrixArray* matrixArrays;
    size_t size;

    if (!loadMatricesFromFile("tests/tests_data/matrix_scale.test", &matrixArrays, &size)) {
        logTestError("Failed to load test data from file", __LINE__);
        return false;
    }

    printf("Beginning matrix scalar testing: %d test case(s)\n", size);

    bool testSuccess = true;
    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* expected = &matrixArrays[testCase].matrices[1];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            testSuccess = false;
            break;
        }

        if(!scaleMatrix(a, 5, &actual)) {
            logTestError("Failed to apply scalar to matrix", __LINE__);
            deleteMatrix(&actual);
            testSuccess = false;
            break;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        deleteMatrix(&actual);

        if (!success) {
            testSuccess = false;
            break;
        }
    }

    freeMatrixArraySet(&matrixArrays, size);
    return testSuccess;
}

bool matrixElementWiseMultiplicationTest() {
    MatrixArray* matrixArrays;
    size_t size;

    if (!loadMatricesFromFile("tests/tests_data/matrix_element_wise_multiplication.test", &matrixArrays, &size)) {
        logTestError("Failed to load test data from file", __LINE__);
        return false;
    }

    printf("Beginning matrix element wise multiplication testing: %d test case(s)\n", size);

    bool testSuccess = true;
    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* b = &matrixArrays[testCase].matrices[1];
        Matrix* expected = &matrixArrays[testCase].matrices[2];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            testSuccess = false;
            break;
        }

        if(!elementWiseMultiplication(a, b, &actual)) {
            logTestError("Failed to perform element wise multiplication of matrices", __LINE__);
            deleteMatrix(&actual);
            testSuccess = false;
            break;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Matrix B:\n");
        displayMatrix(b);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        deleteMatrix(&actual);

        if (!success) {
            testSuccess = false;
            break;
        }
    }

    freeMatrixArraySet(&matrixArrays, size);
    return testSuccess;
}

bool matrixSubtractionTest() {
    MatrixArray* matrixArrays;
    size_t size;

    if (!loadMatricesFromFile("tests/tests_data/matrix_subtraction.test", &matrixArrays, &size)) {
        logTestError("Failed to load test data from file", __LINE__);
        return false;
    }

    printf("Beginning matrix subtraction testing: %d test case(s)\n", size);

    bool testSuccess = true;
    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* b = &matrixArrays[testCase].matrices[1];
        Matrix* expected = &matrixArrays[testCase].matrices[2];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            testSuccess = false;
            break;
        }

        if(!matrixSubtraction(a, b, &actual)) {
            logTestError("Failed to perform matrix subtraction", __LINE__);
            deleteMatrix(&actual);
            testSuccess = false;
            break;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Matrix B:\n");
        displayMatrix(b);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        deleteMatrix(&actual);

        if (!success) {
            testSuccess = false;
            break;
        }
    }

    freeMatrixArraySet(&matrixArrays, size);
    return testSuccess;
}

bool dotProductTransposeTest() {
    MatrixArray* matrixArrays;
    size_t size;

    if (!loadMatricesFromFile("tests/tests_data/dot_product_transpose.test", &matrixArrays, &size)) {
        logTestError("Failed to load test data from file", __LINE__);
        return false;
    }

    printf("Beginning dot product transpose B testing: %d test case(s)\n", size);

    bool testSuccess = true;
    for (size_t testCase = 0; testCase < size; testCase++) {
        Matrix* a = &matrixArrays[testCase].matrices[0];
        Matrix* b = &matrixArrays[testCase].matrices[1];
        Matrix* expected = &matrixArrays[testCase].matrices[2];

        Matrix actual;
        size_t resultRows = expected->columnSize;
        size_t resultCols = expected->rowSize;

        if (!createMatrix(&actual, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
            testSuccess = false;
            break;
        }

        if(!dotProductTransposeB(a, b, &actual)) {
            logTestError("Failed to perform dot product transpose B", __LINE__);
            deleteMatrix(&actual);
            testSuccess = false;
            break;
        }

        printf("Matrix A:\n");
        displayMatrix(a);
        printf("Matrix B:\n");
        displayMatrix(b);
        printf("Result:\n");
        displayMatrix(&actual);

        bool success = areEqualMatrices(expected, &actual);
        printf("Actual equals expected: %s\n", success ? "true" : "false");
        deleteMatrix(&actual);

        if (!success) {
            testSuccess = false;
            break;
        }
    }

    freeMatrixArraySet(&matrixArrays, size);
    return testSuccess;
}