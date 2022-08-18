#ifndef Activations_H
#define Activations_H

#include <stdbool.h>
#include "../matrix/Matrix.h"

// builtin activation functions

bool relu(const Vector* x, Vector* y);
bool dRelu(const Vector* x, Matrix* y);

bool softmax(const Vector* x, Vector* y);
bool dSoftmax(const Vector* x, Matrix* y);

bool linear(const Vector* x, Vector* y);
bool dLinear(const Vector* x, Matrix* y);

// macros for more readable code when using the functions provided above
#define _BUILTIN_RELU {relu, dRelu, true}
#define _BUILTIN_SOFTMAX {softmax, dSoftmax, false}
#define _BUILTIN_NONE {NULL, NULL, false}

#endif