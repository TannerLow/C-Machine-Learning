#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include "../matrix/Matrix.h"
#include "../intdefs.h"

typedef struct {
    bool (*function)(const Vector*, Vector*);
    bool (*derivative)(const Vector*, Matrix*);
    bool elementWiseEligible;
} ActivationFunction;

typedef struct {
    uint16 size;
    ActivationFunction activationFunction;
} LayerParams;

typedef struct {
    Matrix weights;
    Matrix activationInputs;
    Matrix activationOutputs;
    Matrix biases;
    uint16 size;
    ActivationFunction activationFunction;
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
bool copyNetworkStructure(NeuralNet* src, NeuralNet* dst);

bool randomizeWeights(NeuralNet* network);
bool randomizeBiases(NeuralNet* network);

bool setWeights(HiddenLayer* layer, const double* values);
bool setBiases(HiddenLayer* layer, const double* values);

// helper
void deleteHiddenLayer(HiddenLayer* layer);

bool modelPredict(NeuralNet* model, const Vector* input);

#endif // NEURAL_NET_H