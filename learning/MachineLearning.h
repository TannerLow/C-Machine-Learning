#ifndef MACHINE_LEARNING_H
#define MACHINE_LEARNING_H

#include "../matrix/Matrix.h"
#include "../model/NeuralNet.h"

// assumption: expected value for invalid input is 0
double getCost(Matrix* actual, Matrix* expected);
double getAverageCost(Matrix** actuals, Matrix** expecteds, const size_t cases);

typedef struct {
    Matrix weights;
    Matrix biases;
} WeightsBiasesPair;

typedef struct {
    WeightsBiasesPair* layers;
    size_t size;
} LayersUpdates;

// assumption: created matrices will not be 0 set
bool createLayersUpdates(LayersUpdates* updates, NeuralNet* model);
void deleteLayersUpdates(LayersUpdates* updates);
bool addLayersUpdates(LayersUpdates* a, LayersUpdates* b, LayersUpdates* out);
bool clearLayersUpdates(LayersUpdates* updates);

// assumption: activation matrices are 1-Dimensional (N x 1)
bool gradientDescent(NeuralNet* model, Matrix* expected, LayersUpdates* updates);

// assumption: the updates provided are for the model provided
bool updateParameters(NeuralNet* model, LayersUpdates* updates, const double learningRate);

#endif  // MACHINE_LEARNING_H
