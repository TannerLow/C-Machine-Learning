#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include "matrix/Matrix.h"
#include "intdefs.h"

typedef struct {
    uint16 size;
    bool (*activationFunction)(Matrix*, Matrix*);
} LayerParams;

typedef struct {
    Matrix weights;
    Matrix activationInputs;
    Matrix activationOutputs;
    Matrix biases;
    bool (*activationFunction)(Matrix*, Matrix*);
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

// helper
void deleteHiddenLayer(HiddenLayer* layer);

// builtin activation functions
bool relu(Matrix* x, Matrix* y);
bool softmax(Matrix* x, Matrix* y);

#endif // NEURAL_NET_H