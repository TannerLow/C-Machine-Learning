#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include "matrix/Matrix.h"
#include "intdefs.h"

enum ActivationFunction {
    NONE,
    RELU,
    SOFTMAX
};

typedef struct {
    uint16 size;
    ActivationFunction activationFunctionEnum; 
} LayerParams;

typedef struct {
    Matrix weights;
    Matrix activationInputs;
    Matrix activationOutputs;
    Matrix biases;
} HiddenLayer;

// Output layer is just another hidden layer
// it needs weights, biases, and an activation 
// function all the same 
typedef HiddenLayer OutputLayer;

typedef struct {
    Matrix inputLayer;
    HiddenLayer* hiddenLayers;
    OutputLayer outputLayer;
    uint8 totalLayerCount;
} NeuralNet;

// assumption: layerParams is a valid array of the proper size defined by totalLayerCount
bool createNeuralNet(NeuralNet* network, const LayerParams* layerParams, const uint8 totalLayerCount);
void deleteNeuralNet(NeuralNet* network);

#endif // NEURAL_NET_H