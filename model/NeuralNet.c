#include "NeuralNet.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../matrix/MatrixMath.h"

// helper
bool failAndClean(NeuralNet* network) {
    assert(network != NULL);

    deleteNeuralNet(network);
    return false;
}

bool createNeuralNet(NeuralNet* network, const LayerParams* layerParams, const uint8 totalLayerCount) {
    assert(network != NULL);
    assert(layerParams != NULL || totalLayerCount == 2);
    assert(totalLayerCount >= 2);

    // input validation
    if (network == NULL) return false;
    if (layerParams == NULL && totalLayerCount != 2) return false;
    if (totalLayerCount < 2) return false;

    network->totalLayerCount = totalLayerCount;

    // input layer
    uint16 inputSize = layerParams[0].size;
    if (!createMatrix(&network->inputLayer, inputSize, 1)) {
        return failAndClean(network);
    }

    // all other layers
    if (totalLayerCount > 2) {
        uint8 numOfHiddenLayers = totalLayerCount - 2;
        network->hiddenLayers = (HiddenLayer*) malloc(numOfHiddenLayers * sizeof(HiddenLayer));
        if (network->hiddenLayers == NULL) {
            return failAndClean(network);
        }
    }

    for (uint8 i = 1; i < totalLayerCount; i++) {

        HiddenLayer* layer = NULL;
        // output layer is not stored in the hidden layer array
        if (i < totalLayerCount-1) {
            layer = &network->hiddenLayers[i-1];
        }
        else {
            layer = &network->outputLayer;
        }

        // weights matrix is size A(i-1) X A(i)
        // because for every previous layer activation 
        // there is a weight on each current layer activation
        if (!createMatrix(&layer->weights, layerParams[i].size, layerParams[i-1].size)) {
            return failAndClean(network);
        }

        // activation inputs are the Z values
        // that the activation function gets applied to
        // so theres 1 per current layer activation
        if (!createMatrix(&layer->activationInputs, layerParams[i].size, 1)) {
            return failAndClean(network);
        }

        // activation outputs are just the results
        // of applying activation function to activation inputs
        if (!createMatrix(&layer->activationOutputs, layerParams[i].size, 1)) {
            return failAndClean(network);
        }

        // one bias per current layer activation
        if (!createMatrix(&layer->biases, layerParams[i].size, 1)) {
            return failAndClean(network);
        }

        layer->activationFunction = layerParams[i].activationFunction;
        layer->size = layerParams[i].size;
    }

    return true;
}

void deleteNeuralNet(NeuralNet* network) {
    assert(network != NULL);

    if (network != NULL);

    deleteMatrix(&network->inputLayer);

    if (network->hiddenLayers != NULL) {
        for (uint8 i = 1; i < network->totalLayerCount-1; i++) {
            deleteHiddenLayer(&network->hiddenLayers[i-1]);
        }
    }

    deleteHiddenLayer(&network->outputLayer);

    network->totalLayerCount = 0;
}

void deleteHiddenLayer(HiddenLayer* layer) {
    assert(layer != NULL);

    if (layer != NULL) {
        deleteMatrix(&layer->weights);
        deleteMatrix(&layer->activationInputs);
        deleteMatrix(&layer->activationOutputs);
        deleteMatrix(&layer->biases);
        layer->activationFunction = NULL;
    }
}

bool modelPredict(NeuralNet* model, const Vector* input) {
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