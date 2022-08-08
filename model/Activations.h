#ifndef Activations_H
#define Activations_H

#include <stdbool.h>
#include "../matrix/Matrix.h"

// builtin activation functions
bool relu(const Vector* x, Vector* y);
bool softmax(const Vector* x, Vector* y);
bool linear(const Vector* x, Vector* y);

#endif