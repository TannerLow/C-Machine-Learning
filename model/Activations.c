#include "Activations.h"
#include <math.h>
#include <assert.h>

// helper
double max(const double a, const double b) {
    return a >= b ? a : b;
}

bool relu(const Vector* x, Vector* y) {
    assert(x != NULL && y != NULL);

    // input validation
    if (x == NULL || y == NULL) return false; // invalid operands 
    if (!areEqualSizedVectors(x, y)) return false; // invalid dimensions

    // actual math
    for (size_t i = 0; i < x->size; i++) {
        y->data[i] = max(0, x->data[i]);
    }    

    return true;
}

bool softmax(const Vector* x, Vector* y) {
    assert(x != NULL && y != NULL);

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

bool linear(const Vector* x, Vector* y) {
    assert(x != NULL && y != NULL);

    return copyVector(x, y);
}