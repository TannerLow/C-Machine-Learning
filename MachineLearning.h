#ifndef MACHINE_LEARNING_H
#define MACHINE_LEARNING_H

#include <stdbool.h>
#include "matrix/Matrix.h"
#include "NeuralNet.h"

bool predict(NeuralNet* model, const Vector* input);

// builtin activation functions
bool relu(const Vector* x, Vector* y);
bool softmax(const Vector* x, Vector* y);

#endif
