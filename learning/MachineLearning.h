#ifndef MACHINE_LEARNING_H
#define MACHINE_LEARNING_H

#include "../matrix/Matrix.h"
#include "../model/NeuralNet.h"

// assumption: expected value for invalid input is 0
double getCost(Matrix* actual, Matrix* expected);
double getAverageCost(Matrix** actuals, Matrix** expecteds, const size_t cases);

// assumption: activation matrices are 1-Dimensional (N x 1)
bool optimizeSGD(NeuralNet* model, Matrix* expected, const double learningRate);

#endif  // MACHINE_LEARNING_H
