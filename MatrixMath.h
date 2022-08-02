#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include "Matrix.h"
#include <stdbool.h>

bool areEqualSizes(Dimensions2D a, Dimensions2D b);

bool dotProduct(Matrix* a, Matrix* b, Matrix* result);
bool dotProductTransposeB(Matrix* a, Matrix* b, Matrix* result);
bool elementWiseMultiplication(Matrix* a, Matrix* b, Matrix* result);
bool matrixAddition(Matrix* a, Matrix* b, Matrix* result);
bool matrixSubtraction(Matrix* a, Matrix* b, Matrix* result);

#endif // MATRIX_MATH_H