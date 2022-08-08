#include "MatrixDebug.h"
#include <stdio.h>

void displayMatrix(const Matrix* m) {
    for (size_t row = 0; row < m->columnSize; row++) {
        for (size_t col = 0; col < m->rowSize; col++) {
            printf("%lf ", getMatrixElement(m, row, col));
        }
        printf("\n");
    }
}