#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include "Matrix.h"
#include <stdbool.h>

bool dotProduct(Matrix* a, Matrix* b, Matrix* result);
bool dotProductTransposeB(Matrix* a, Matrix* b, Matrix* result);
bool elementWiseMultiplication(Matrix* a, Matrix* b, Matrix* result);
bool matrixAddition(Matrix* a, Matrix* b, Matrix* result);
bool matrixSubtraction(Matrix* a, Matrix* b, Matrix* result);
bool scaleMatrix(Matrix* a, double scalar, Matrix* result);

// assumption: max size_t is expected value for invalid vector
size_t maxIndex(const Vector* vector);

#endif // MATRIX_MATH_H