#include "MachineLearning.h"
#include "matrix/MatrixMath.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>

bool predict(NeuralNet* model, const Vector* input) {
    assert(model != NULL && input != NULL);
    assert(model->hiddenLayers != NULL);
    assert(input->data != NULL);
    assert(input->size > 0);
    assert(input->size == model->inputLayer.columnSize);

    // input validation
    if ((model == NULL || input == NULL) ||
        (model->hiddenLayers == NULL)    ||
        (input->data == NULL)            ||
        (input->size == 0)                ||
        (input->size != model->inputLayer.columnSize))
    {
        return false;
    }

    if (!copyVector(input, &model->inputLayer.columns[0])) {
        return false; // failed to copy input into model
    }

    Matrix* previousActivations = &model->inputLayer;

    for (uint8 i = 1; i < model->totalLayerCount; i++) {

        HiddenLayer* layer = NULL;
        // output layer is not stored in the hidden layer array
        if (i < model->totalLayerCount-1) {
            layer = &model->hiddenLayers[i-1];
        }
        else {
            layer = &model->outputLayer;
        }

        // A(i) = A(i-1) X W + B
        // Ao(i) = f(A(i))

        // A(i) = A(i-1) X W
        if (!dotProduct(&layer->weights, previousActivations, &layer->activationInputs)) {
            return false; // failed to apply weights to previous layer outputs
        }

        // A(i) = A(i) + B
        if (!matrixAddition(&layer->activationInputs, &layer->biases, &layer->activationInputs)) {
            return false; // failed to add in the biases
        }

        // Ao(i) = f(A(i))
        if (!layer->activationFunction(&layer->activationInputs.columns[0], &layer->activationOutputs.columns[0])) {
            return false; // failed to apply activation functions
        }

        previousActivations = &layer->activationOutputs;
    }

    return true;
}

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