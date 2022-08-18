#include "MatrixMath.h"
#include <assert.h>
#include <stdio.h>
#include <float.h>


bool dotProduct(Matrix* a, Matrix* b, Matrix* result) {
    assert(a != NULL && b != NULL);
    printf("%llu x %llu * %llu x %llu\n", a->columnSize, a->rowSize, b->columnSize, b->rowSize);
    assert(a->rowSize == b->columnSize);

    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (a->rowSize != b->columnSize) return false; // invalid dimensions

    Dimensions2D expectedDimensions = { a->columnSize, b->rowSize };
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) {
        return false; // result dimensions unexpected
    }

    // actual math
    for (size_t aRow = 0; aRow < a->columnSize; aRow++) {
        for(size_t bCol = 0; bCol < b->rowSize; bCol++) {
            // dimensions must align so we can iterate with only 1 variable
            double sum = 0;
            for (size_t i = 0; i < a->rowSize; i++) {
                sum += getMatrixElement(a, aRow, i) * getMatrixElement(b, i, bCol);
            }
            if(!setMatrixElement(result, aRow, bCol, sum)) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

#include "../matrix/MatrixDebug.h"

bool dotProductTransposeB(Matrix* a, Matrix* b, Matrix* result) {
    assert(a != NULL && b != NULL);
    assert(a->rowSize == b->rowSize);

    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (a->rowSize != b->rowSize) return false; // invalid dimensions

    printf("\ndotProductTransposeB\n");
    displayMatrix(a);
    printf("\n");
    displayMatrix(b);
    printf("\n");
    printf("result size: %llu x %llu\n", result->columnSize, result->rowSize);

    Dimensions2D expectedDimensions = { a->columnSize, b->columnSize };
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) {
        printf("actual  : %llu %llu\n", actualDimensions.rows, actualDimensions.cols);
        printf("expected: %llu %llu\n", expectedDimensions.rows, expectedDimensions.cols);
        assert(!"actual dimensions didn't match expected dimensions\n");
        return false; // result dimensions unexpected
    }

    // actual math
    for (size_t aRow = 0; aRow < a->columnSize; aRow++) {
        for(size_t bRow = 0; bRow < b->columnSize; bRow++) {
            // dimensions must align so we can iterate with only 1 variable
            double sum = 0;
            for (size_t col = 0; col < a->rowSize; col++) {
                sum += getMatrixElement(a, aRow, col) * getMatrixElement(b, bRow, col);
            }
            if(!setMatrixElement(result, aRow, bRow, sum)) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

bool dotProductTransposeA(Matrix* a, Matrix* b, Matrix* result) {
    printf("\ndotProductTransposeA\n");
    displayMatrix(a);
    printf("\n");
    displayMatrix(b);
    printf("\n");
    printf("result size: %llu x %llu\n", result->columnSize, result->rowSize);

    assert(a != NULL && b != NULL);
    assert(a->columnSize == b->columnSize);

    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (a->columnSize != b->columnSize) return false; // invalid dimensions

    Dimensions2D expectedDimensions = { a->rowSize, b->rowSize };
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) {
        return false; // result dimensions unexpected
    }

    // actual math
    for (size_t aCol = 0; aCol < a->rowSize; aCol++) {
        for(size_t bCol = 0; bCol < b->rowSize; bCol++) {
            // dimensions must align so we can iterate with only 1 variable
            double sum = 0;
            for (size_t row = 0; row < a->columnSize; row++) {
                sum += getMatrixElement(a, row, aCol) * getMatrixElement(b, row, bCol);
            }
            if(!setMatrixElement(result, aCol, bCol, sum)) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

bool elementWiseMultiplication(Matrix* a, Matrix* b, Matrix* result) {
    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (!areEqualSizes(getDimensions(a), getDimensions(b))) return false; // invalid dimensions

    Dimensions2D expectedDimensions = getDimensions(a);
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) {
        return false; // result dimensions unexpected
    }

    // actual math
    for (size_t row = 0; row < a->columnSize; row++) {
        for(size_t col = 0; col < a->rowSize; col++) {

            double value = getMatrixElement(a, row, col) * getMatrixElement(b, row, col);

            if(!setMatrixElement(result, row, col, value)) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

bool matrixAddition(Matrix* a, Matrix* b, Matrix* result) {
    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (!areEqualSizes(getDimensions(a), getDimensions(b))) return false; // invalid dimensions

    Dimensions2D expectedDimensions = getDimensions(a);
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) {
        return false; // result dimensions unexpected
    }

    // actual math
    for (size_t row = 0; row < a->columnSize; row++) {
        for(size_t col = 0; col < a->rowSize; col++) {

            double value = getMatrixElement(a, row, col) + getMatrixElement(b, row, col);

            if(!setMatrixElement(result, row, col, value)) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

bool matrixSubtraction(Matrix* a, Matrix* b, Matrix* result) {
    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (!areEqualSizes(getDimensions(a), getDimensions(b))) return false; // invalid dimensions

    Dimensions2D expectedDimensions = getDimensions(a);
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) {
        return false; // result dimensions unexpected
    }

    // actual math
    for (size_t row = 0; row < a->columnSize; row++) {
        for(size_t col = 0; col < a->rowSize; col++) {

            double value = getMatrixElement(a, row, col) - getMatrixElement(b, row, col);

            if(!setMatrixElement(result, row, col, value)) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

bool scaleMatrix(Matrix* a, double scalar, Matrix* result) {
    // input validation
    if (a == NULL || result == NULL) return false; // invalid operands 

    if (!areEqualSizes(getDimensions(a), getDimensions(result))) return false; // invalid dimensions

    // actual math
    for (size_t row = 0; row < a->columnSize; row++) {
        for(size_t col = 0; col < a->rowSize; col++) {          
            if(!setMatrixElement(result, row, col, scalar * getMatrixElement(a, row, col))) {
                return false; // attempt to index out of bounds
            }
        }
    }

    return true;
}

size_t maxIndex(const Vector* vector) {
    assert(vector != NULL);
    
    size_t index = (size_t)-1;
    double largestVal = DBL_MIN;

    if (vector->data == NULL) return index;

    for (size_t i = 0; i < vector->size; i++) {
        if (vector->data[i] > largestVal) {
            largestVal = vector->data[i];
            index = i;
        }
    }
    
    return index;
}