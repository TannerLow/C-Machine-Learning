#include "MatrixDebug.h"
#include <stdio.h>

void displayMatrix(const Matrix* m) {
    for (size_t row = 0; row < m->columnSize; row++) {
        for (size_t col = 0; col < m->rowSize; col++) {
            double value = getMatrixElement(m, row, col);
            if (value < 0) {
                printf("%.6lf", getMatrixElement(m, row, col));
            }
            else {
                printf(" %.6lf", getMatrixElement(m, row, col));
            }
            if (col != m->rowSize-1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}