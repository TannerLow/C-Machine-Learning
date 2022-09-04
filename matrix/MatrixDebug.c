#include "MatrixDebug.h"
#include <stdio.h>
#include "../logger/logger.h"

void displayMatrix(const Matrix* m) {
    //printf("rows: %llu, cols: %llu\n", m->columnSize, m->rowSize);
    for (size_t row = 0; row < m->columnSize; row++) {
        for (size_t col = 0; col < m->rowSize; col++) {
            //printf("(%llu, %llu) ", row, col);
            double value = getMatrixElement(m, row, col);
            //printf("%lf = \n", value);
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

void logMatrix(const Matrix* m) {
    //fprintf(global_logger.file, "rows: %llu, cols: %llu\n", m->columnSize, m->rowSize);
    for (size_t row = 0; row < m->columnSize; row++) {
        for (size_t col = 0; col < m->rowSize; col++) {
            //fprintf(global_logger.file, "(%llu, %llu) ", row, col);
            double value = getMatrixElement(m, row, col);
            //fprintf(global_logger.file, "%lf = \n", value);
            if (value < 0) {
                fprintf(global_logger.file, "%.6lf", getMatrixElement(m, row, col));
            }
            else {
                fprintf(global_logger.file, " %.6lf", getMatrixElement(m, row, col));
            }
            if (col != m->rowSize-1) {
                fprintf(global_logger.file, ", ");
            }
        }
        fprintf(global_logger.file, "\n");
    }
}