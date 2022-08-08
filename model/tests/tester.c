#include "tester.h"
#include "../NeuralNet.h"
#include <stdio.h>
#include <assert.h>
#include "../../intdefs.h"
#include <stdlib.h>
#include "../Activations.h"
#include "../../matrix/MatrixDebug.h"

bool loadModelFromFile(NeuralNet* model, Vector* input, Matrix* expected, const char* filename);

bool model_testAll() {
    bool prediction = model_testPrediction();

    printf("Model prediction test: %s\n", prediction ? "success" : "FAILURE");

    return prediction;
}

// assumption: the file is formatted correctly
bool loadModelFromFile(NeuralNet* model, Vector* input, Matrix* expected, const char* filename) {
    assert(model != NULL);
    assert(input != NULL);
    assert(expected != NULL);
    assert(filename != NULL);

    if (model == NULL    ||
        input == NULL    ||
        expected == NULL ||
        filename == NULL) 
    {
        return false; // invalid inputs
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) return false; // unable to open file

// Need to acquire data needed to create model first
    uint8 layerCount;
    int readCount = fscanf(file, "%d", &layerCount);
    if (readCount != 1) return false; // failed read from file

    LayerParams* layerParams = (LayerParams*) malloc(layerCount * sizeof(LayerParams));
    if (layerParams == NULL) return false; // failed to allocate memory for layer parameters

    for (uint8 i = 0; i < layerCount; i++) {
        readCount = fscanf(file, "%d %d", &layerParams[i].size, &layerParams[i].activationFunction);
        if (readCount != 2) {
            free(layerParams);
            return false; // failed file read while acquiring layer parameter data
        }
    }

// Create the model
    bool modelCreated = createNeuralNet(model, layerParams, layerCount);
    free(layerParams);
    if(!modelCreated) {
        return false; // failed to create model
    }

// Need to read and set weights and biases of each layer
    uint16 previousLayerSize = model->inputLayer.columnSize;
    for (uint8 i = 1; i < layerCount; i++) {

        HiddenLayer* layer;
        // output layer is not stored in the hidden layer array
        if (i < layerCount-1) {
            layer = &model->hiddenLayers[i-1];
        }
        else {
            layer = &model->outputLayer;
        }

        Matrix* weights = &layer->weights;
        Matrix* biases = &layer->biases;

        // set the weights
        for (size_t row = 0; row < weights->columnSize; row++) {
            for (size_t col = 0; col < weights->rowSize; col++) {
                double val = 0;

                readCount = fscanf(file, "%lf", &val);
                if (readCount != 1) {
                    deleteNeuralNet(model);
                    return false; // failed to read in a weight value
                }

                setMatrixElement(weights, row, col, val);
            }
        }

        // set the biases
        for (size_t row = 0; row < biases->columnSize; row++) {
            double val = 0;

            readCount = fscanf(file, "%lf", &val);
            if (readCount != 1) {
                deleteNeuralNet(model);
                return false; // failed to read in a bias value
            }

            setMatrixElement(biases, row, 0, val);
        }

        // set the activation function
        switch((size_t)layer->activationFunction) {
            case 0: layer->activationFunction = linear; break;
            case 1: layer->activationFunction = relu; break;
            case 2: layer->activationFunction = softmax; break;
            default: layer->activationFunction = linear;
        }
    }

// Need to create the vector to input to the model prediction
    if (!createVector(input, model->outputLayer.size)) {
        deleteNeuralNet(model);
        return false; // failed to create matrix to store expected output
    }

    for (uint16 i = 0; i < input->size; i++) {
        double val = 0;

        readCount = fscanf(file, "%lf", &val);
        if (readCount != 1) {
            deleteNeuralNet(model);
            deleteVector(input);
            return false; // failed to read in an expected value
        }

        setVectorElement(input, i, val);
    }

// Need to create an expected output matrix for verifying correctness
    if (!createMatrix(expected, model->outputLayer.size, 1)) {
        deleteNeuralNet(model);
        deleteVector(input);
        return false; // failed to create matrix to store expected output
    }

    for (uint16 i = 0; i < expected->columnSize; i++) {
        double val = 0;

        readCount = fscanf(file, "%lf", &val);
        if (readCount != 1) {
            deleteNeuralNet(model);
            deleteVector(input);
            deleteMatrix(expected);
            return false; // failed to read in an expected value
        }

        setMatrixElement(expected, i, 0, val);
    }

    fclose(file);
    return true;
}

void testPredictionClean(NeuralNet* model, Matrix* expected ,Vector* input) {
    assert(model != NULL);
    assert(expected != NULL);
    assert(input != NULL);

    if (model != NULL)    deleteNeuralNet(model);
    if (expected != NULL) deleteMatrix(expected);
    if (input != NULL)    deleteVector(input);
}

bool model_testPrediction() {
    printf("[TEST] %s, %d\n", __FILE__, __LINE__);

    NeuralNet model;
    Matrix expected;
    Vector input;

    if(!loadModelFromFile(&model, &input, &expected, "model/tests/tests_data/prediction.test")) {
        printf("Failed to load Test data\n");
        return false; // failed to load model and/or expected output
    }

    if (!modelPredict(&model, &input)) {
        printf("Failed to perform prediction\n");
        testPredictionClean(&model, &expected, &input);
        return false; // failed to perform prediction using model
    }

    printf("Expected:\n");
    displayMatrix(&model.outputLayer.activationOutputs);

    printf("Actual:\n");
    displayMatrix(&expected);

    // Verify that the output is the same as the expected
    if (!areEqualMatrices(&model.outputLayer.activationOutputs, &expected)) {
        printf("Actual output did not match expectations\n");
        testPredictionClean(&model, &expected, &input);
        return false; // model prediction results not as expected
    }

    return true;
}