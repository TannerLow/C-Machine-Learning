#include "MachineLearning.h"
#include "../matrix/MatrixMath.h"
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "../matrix/MatrixDebug.h"

// NOTE: can const qualify if I make an in-place subtraction function
double getCost(Matrix* actual, Matrix* expected) {
    assert(actual != NULL && expected != NULL);

    double cost = 0;
    if (actual != NULL && expected != NULL) {
        Matrix result;

        // cost = 1/2 * summation[ (e[i] - a[i])^2 ]
        if (createMatrix(&result, actual->columnSize, actual->rowSize) && matrixSubtraction(expected, actual, &result)) {
            for (size_t col = 0; col < result.rowSize; col++) {
                for (size_t row = 0; row < result.columnSize; row++) {
                    cost += pow(getMatrixElement(&result, row, col), 2);
                }
            }
        }

    }

    return cost / 2.0;
}

double getAverageCost(Matrix** actuals, Matrix** expecteds, const size_t cases) {
    assert(actuals != NULL && expecteds != NULL);
    assert(cases > 0);

    double averageCost = 0;
    if (actuals != NULL && expecteds != NULL && cases != 0) {

        // avg cost = summation[ cost(a[i], e[i]) ] / n
        for (size_t i = 0; i < cases; i++) {
            averageCost += getCost(actuals[i], expecteds[i]);
        }
        averageCost /= cases;
    }

    return averageCost;
}

// TODO: clean up temporary matrices during this function
bool optimizeSGD(NeuralNet* model, Matrix* expected, const double learningRate) {
    assert(model != NULL);
    assert(model->hiddenLayers != NULL || model->totalLayerCount == 2);
    assert(expected != NULL);
    assert(areEqualSizes(getDimensions(expected), getDimensions(&model->outputLayer.activationOutputs)));

    if((model == NULL) ||
       (model->hiddenLayers == NULL && model->totalLayerCount != 2) ||
       (expected == NULL) ||
       (!areEqualSizes(getDimensions(expected), getDimensions(&model->outputLayer.activationOutputs)))
    ) {
        printf("Input validation failed (%s, %d)\n", __FILE__, __LINE__);
        return false;
    }

    NeuralNet deltaNet;
    assert(copyNetworkStructure(model, &deltaNet));

    // L = last layer; l = hidden layer
    // dC/dA[L]
    assert(matrixSubtraction(&model->outputLayer.activationOutputs, expected, &deltaNet.outputLayer.activationOutputs));

    // dA[L]/dZ[L]
    Matrix jacobian;
    size_t rows = model->outputLayer.size;
    if (model->outputLayer.activationFunction.elementWiseEligible) {
        assert(createMatrix(&jacobian, rows, 1));
    }
    else {
        assert(createMatrix(&jacobian, rows, rows));
    }
    assert(model->outputLayer.activationFunction.derivative(&model->outputLayer.activationInputs.columns[0], &jacobian));

    // dC/dZ[L]
    if (model->outputLayer.activationFunction.elementWiseEligible) {
        assert(elementWiseMultiplication(&jacobian, &deltaNet.outputLayer.activationOutputs, &deltaNet.outputLayer.activationInputs));
    }
    else {
        assert(dotProductTransposeA(&jacobian, &deltaNet.outputLayer.activationOutputs, &deltaNet.outputLayer.activationInputs));
    }
    deleteMatrix(&jacobian);

    // dZ[L]/dW[L]
    Matrix temp;
    uint8 layerIdx = model->totalLayerCount- 2 - 1; // -2 for input and output layers
    assert(createMatrix(&temp, model->hiddenLayers[layerIdx].size, 1));
    assert(copyMatrix(&model->hiddenLayers[layerIdx].activationOutputs, &temp));

    // dC/dW[L]
    assert(dotProductTransposeB(&deltaNet.outputLayer.activationInputs, &temp, &deltaNet.outputLayer.weights));
    deleteMatrix(&temp);

    // dC/dB[L]
    assert(copyMatrix(&deltaNet.outputLayer.activationInputs, &deltaNet.outputLayer.biases));

    // the rest
    uint8 outputLayerIdx = layerIdx + 1;
    for (/*layerIdx*/; layerIdx < model->totalLayerCount; layerIdx--) {
        printf("layer: %d\n", layerIdx);
        Matrix* previousLayerOutputs = NULL;
        if (layerIdx != 0) {
            previousLayerOutputs = &model->hiddenLayers[layerIdx-1].activationOutputs;
        }
        else {
            previousLayerOutputs = &model->inputLayer;

        }

        HiddenLayer* layer = &model->hiddenLayers[layerIdx];
        
        // dZ[l]/dA[l] = W[l+1]
        HiddenLayer* nextLayer = (layerIdx+1 == outputLayerIdx) ? &model->outputLayer : &model->hiddenLayers[layerIdx+1];
        HiddenLayer* deltasOfNextLayer = (layerIdx+1 == outputLayerIdx) ? &deltaNet.outputLayer : &deltaNet.hiddenLayers[layerIdx+1];;
        
        // dC/dA[l]
        assert(dotProductTransposeA(&nextLayer->weights, &deltasOfNextLayer->activationInputs, &deltaNet.hiddenLayers[layerIdx].activationOutputs));

        // dA[l]/dZ[l]
        rows = layer->size;
        if (layer->activationFunction.elementWiseEligible) {
            assert(createMatrix(&jacobian, rows, 1));
        }
        else {
            assert(createMatrix(&jacobian, rows, rows));
        }
        assert(layer->activationFunction.derivative(&layer->activationInputs.columns[0], &jacobian));

        // dC/dZ[l]
        if (layer->activationFunction.elementWiseEligible) {
            assert(elementWiseMultiplication(&jacobian, &deltaNet.hiddenLayers[layerIdx].activationOutputs, &deltaNet.hiddenLayers[layerIdx].activationInputs));
        }
        else {
            assert(dotProductTransposeA(&jacobian, &deltaNet.hiddenLayers[layerIdx].activationOutputs, &deltaNet.hiddenLayers[layerIdx].activationInputs));
        }
        deleteMatrix(&jacobian);

        // dZ[l]/dW[l]
        assert(createMatrix(&temp, previousLayerOutputs->columnSize, 1));
        assert(copyMatrix(previousLayerOutputs, &temp));

        // dC/W[l]
        assert(dotProductTransposeB(&deltaNet.hiddenLayers[layerIdx].activationInputs, &temp, &deltaNet.hiddenLayers[layerIdx].weights));
        deleteMatrix(&temp);

        // dC/dB[l]
        assert(copyMatrix(&deltaNet.hiddenLayers[layerIdx].activationInputs, &deltaNet.hiddenLayers[layerIdx].biases));
    }

    // update weights and biases
    assert(matrixSubtraction(&model->outputLayer.weights, &deltaNet.outputLayer.weights, &model->outputLayer.weights));
    assert(matrixSubtraction(&model->outputLayer.biases, &deltaNet.outputLayer.biases, &model->outputLayer.biases));

    for (uint8 layerIdx = 0; layerIdx < deltaNet.totalLayerCount-2; layerIdx++) { // -2 for input and output layer
        assert(matrixSubtraction(&model->hiddenLayers[layerIdx].weights, &deltaNet.hiddenLayers[layerIdx].weights, &model->hiddenLayers[layerIdx].weights));
        assert(matrixSubtraction(&model->hiddenLayers[layerIdx].biases, &deltaNet.hiddenLayers[layerIdx].biases, &model->hiddenLayers[layerIdx].biases));
    }

    deleteNeuralNet(&deltaNet);

    return true;
}