#include "MatrixMath.h"

bool areEqualSizes(Dimensions2D a, Dimensions2D b) {
    return a.rows == b.rows && a.cols == b.cols;
}

bool dotProduct(Matrix* a, Matrix* b, Matrix* result) {
    // input validation
    if (a == NULL || b == NULL || result == NULL) return false; // invalid operands 

    if (a->columnSize != b->rowSize) return false; // invalid dimensions

    Dimensions2D expectedDimensions = { a->columnSize, b->rowSize };
    Dimensions2D actualDimensions = getDimensions(result);
    if (!areEqualSizes(actualDimensions, expectedDimensions)) 
        return false; // result dimensions unexpected

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

bool dotProductTransposeB(Matrix* a, Matrix* b, Matrix* result) {
    return false;
}

bool elementWiseMultiplication(Matrix* a, Matrix* b, Matrix* result) {
    return false;
}

bool matrixAddition(Matrix* a, Matrix* b, Matrix* result) {
    return false;
}

bool matrixSubtraction(Matrix* a, Matrix* b, Matrix* result) {
    return false;
}