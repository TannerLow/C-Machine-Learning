#include "tester.h"
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "MatrixMath.h"

void logTestError(const char* msg, int line);
bool loadMatricesFromFile(const char* filename, Matrix** output, unsigned int& size);
void displayMatrix(Matrix* m);
void dotProductTest();

void test() {
    dotProductTest();
    printf("Done Testing\n");
}

void logTestError(const char* msg, int line) {
    printf("%s (%s, %d)\n", msg, __FILE__, line);
}

bool loadMatricesFromFile(const char* filename, Matrix** output, unsigned int& size) {
    FILE* filePtr = fopen(filename, "r");
    
    if (filePtr == NULL) {
        return false;
    }

    fscanf(filePtr, "%d", &size);
    

    while (N--) {
        Matrix a, b, c;

        int resultRows, resultCols;
        int aRows, aCols;

        fscanf(filePtr, "%d %d", &aRows, &aCols);
        resultRows = aRows;
        if (!createMatrix(&a, aRows, aCols)) {
            logTestError("Failed to create matrix", __LINE__);
        }

        printf("Rows: %d, Cols: %d\n", aRows, aCols);
        for (int row = 0; row < aRows; row++) {
            for(int col = 0; col < aCols; col++) {
                double value;
                fscanf(filePtr, "%lf", &value);
                printf("%lf ", value);
                setMatrixElement(&a, row, col, value);
            }
            printf("\n");
        }

        int bRows, bCols;

        fscanf(filePtr, "%d %d", &bRows, &bCols);
        resultCols = bCols;
        if (!createMatrix(&b, bRows, bCols)) {
            logTestError("Failed to create matrix", __LINE__);
        }

        printf("Rows: %d, Cols: %d\n", bRows, bCols);
        for (int row = 0; row < bRows; row++) {
            for(int col = 0; col < bCols; col++) {
                double value;
                fscanf(filePtr, "%lf", &value);
                printf("%lf ", value);
                setMatrixElement(&b, row, col, value);
            }
            printf("\n");
        }

        printf("Rows: %d, Cols: %d\n", resultRows, resultCols);
        for (int row = 0; row < resultRows; row++) {
            for(int col = 0; col < resultCols; col++) {
                double value;
                fscanf(filePtr, "%lf", &value);
                printf("%lf ", value);
            }
            printf("\n");
        }
    }
    // free output if possible then save the loaded matrixes
}

void displayMatrix(Matrix* m) {
    for (size_t row = 0; row < m->columnSize; row++) {
        for (size_t col = 0; col < m->rowSize; col++) {
            printf("%lf ", getMatrixElement(m, row, col));
        }
        printf("\n");
    }
}

void dotProductTest() {
    FILE* filePtr = fopen("dot_product.test", "r");
    int N = 0;
    fscanf(filePtr, "%d", &N);

    while (N--) {
        Matrix a, b, c;

        int resultRows, resultCols;
        int aRows, aCols;

        fscanf(filePtr, "%d %d", &aRows, &aCols);
        resultRows = aRows;
        if (!createMatrix(&a, aRows, aCols)) {
            logTestError("Failed to create matrix", __LINE__);
        }

        printf("Rows: %d, Cols: %d\n", aRows, aCols);
        for (int row = 0; row < aRows; row++) {
            for(int col = 0; col < aCols; col++) {
                double value;
                fscanf(filePtr, "%lf", &value);
                printf("%lf ", value);
                setMatrixElement(&a, row, col, value);
            }
            printf("\n");
        }

        int bRows, bCols;

        fscanf(filePtr, "%d %d", &bRows, &bCols);
        resultCols = bCols;
        if (!createMatrix(&b, bRows, bCols)) {
            logTestError("Failed to create matrix", __LINE__);
        }

        printf("Rows: %d, Cols: %d\n", bRows, bCols);
        for (int row = 0; row < bRows; row++) {
            for(int col = 0; col < bCols; col++) {
                double value;
                fscanf(filePtr, "%lf", &value);
                printf("%lf ", value);
                setMatrixElement(&b, row, col, value);
            }
            printf("\n");
        }

        printf("Rows: %d, Cols: %d\n", resultRows, resultCols);
        for (int row = 0; row < resultRows; row++) {
            for(int col = 0; col < resultCols; col++) {
                double value;
                fscanf(filePtr, "%lf", &value);
                printf("%lf ", value);
            }
            printf("\n");
        }

        if (!createMatrix(&c, resultRows, resultCols)) {
            logTestError("Failed to create matrix", __LINE__);
        }

        if(!dotProduct(&a, &b, &c)) {
            logTestError("Failed to perform dot product", __LINE__);
        }

        printf("Matrix A:\n");
        displayMatrix(&a);
        printf("Matrix B:\n");
        displayMatrix(&b);
        printf("Result:\n");
        displayMatrix(&c);
    }

    fclose(filePtr);
}