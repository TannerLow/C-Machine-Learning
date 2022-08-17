#include "Activations.h"
#include <math.h>
#include <assert.h>

// helper
double max(const double a, const double b) {
    return a >= b ? a : b;
}

bool relu(const Vector* x, Vector* y) {
    assert(x != NULL && y != NULL);
    assert(areEqualSizedVectors(x, y));

    // input validation
    if (x == NULL || y == NULL) return false; // invalid operands 
    if (!areEqualSizedVectors(x, y)) return false; // invalid dimensions

    // actual math
    for (size_t i = 0; i < x->size; i++) {
        y->data[i] = max(0, x->data[i]);
    }    

    return true;
}

bool dRelu(const Vector* x, Matrix* y) {
    assert(x != NULL && y != NULL);
    assert(x->size == y->columnSize);

    // input validation
    if (x == NULL || y == NULL) return false; // invalid operands 
    if (x->size != y->columnSize) return false; // invalid dimensions

    // actual math
    for (size_t i = 0; i < x->size; i++) {
        setMatrixElement(y, i, 0, x->data[i] > 0 ? 1 : 0);
    }    

    return true;
}

bool softmax(const Vector* x, Vector* y) {
    assert(x != NULL && y != NULL);
    assert(areEqualSizedVectors(x, y));

    // input validation
    if (x == NULL || y == NULL) return false; // invalid operands
    if (!areEqualSizedVectors(x, y)) return false; // invalid dimensions

    // actual math
    // denominator summation
    double sum = 0;
    for (size_t i = 0; i < x->size; i++) {
        sum += exp(x->data[i]);
    }
    // output calculation
    for (size_t i = 0; i < x->size; i++) {
        y->data[i] = exp(x->data[i]) / sum;
    } 

    return true;
}

bool dSoftmax(const Vector* x, Matrix* y) {
    assert(x != NULL && y != NULL);
    assert(x->size == y->columnSize);
    assert(x->size == y->rowSize);

    // input validation
    if (x == NULL || y == NULL) return false; // invalid operands
    if (x->size != y->columnSize) return false; // invalid dimensions
    if (x->size != y->rowSize) return false; // invalid dimensions

    Vector softmaxes;
    if (!createVector(&softmaxes, x->size)) {
        return false; // failed to allocate memory for softmaxes
    }

    // actual math
    if (!softmax(x, &softmaxes)) {
        return false; // failed to calculate softmax for all x's
    }

    // output calculation
    for (size_t col = 0; col < y->rowSize; col++) {
        for (size_t row = 0; row < y->columnSize; row++) {
            if (col == row) {
                setMatrixElement(y, row, col, softmaxes.data[col] * (1.0 - softmaxes.data[col]));
            }
            else {
                setMatrixElement(y, row, col, -1.0 * softmaxes.data[col] * softmaxes.data[row]);
            }
        }
    }

    return true;
}

bool linear(const Vector* x, Vector* y) {
    assert(x != NULL && y != NULL);
    assert(areEqualSizedVectors(x, y));

    if (x == NULL || y == NULL) return false;
    if (!areEqualSizedVectors(x, y)) return false; // invalid dimensions

    return copyVector(x, y);
}

bool dLinear(const Vector* x, Matrix* y) {
    assert(x != NULL && y != NULL);
    assert(x->size == y->columnSize);

    if (x == NULL || y == NULL) return false;
    if (x->size != y->columnSize) return false; // invalid dimensions

    for (size_t i = 0; i < x->size; i++) {
        setMatrixElement(y, i, 0, 1);
    }

    return true;
}