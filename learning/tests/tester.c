#include "tester.h"
#include "../MachineLearning.h"
#include "../../matrix/Matrix.h"
#include "../../model/NeuralNet.h"
#include "../../intdefs.h"
#include "../../model/Activations.h"
#include <stdio.h>
#include <assert.h>
#include "../../matrix/MatrixDebug.h"

bool ml_testAll() {
    bool cost = ml_testCost();
    bool averageCost = ml_testAverageCost();
    bool sgd = ml_testSGD();

    printf("Cost Test        : %s\n", cost ? "success" : "FAILURE");
    printf("Average Cost Test: %s\n", averageCost ? "success" : "FAILURE");
    printf("Optimize SGD Test: %s\n", sgd ? "success" : "FAILURE");
    printf("\n");

    bool b = cost 
        && averageCost 
        && sgd;

    assert(b);
    return b;
}

void ml_createCostTestVector(Vector* vector, double* data, const size_t size) {
    assert(vector != NULL);
    assert(data != NULL);
    assert(size != 0);

    vector->data = data;
    vector->size = size;
}

void ml_createCostTestMatrix(Matrix* matrix, Vector* data) {
    assert(matrix != NULL);
    assert(data != NULL);

    matrix->columns = data;
    matrix->columnSize = data->size;
    matrix->rowSize = 1;
}

bool ml_testCost() {
    printf("[TEST] %s, %d\n", __FILE__, __LINE__);
    const double expectedCost = 6.5;
    double actualData[] = {6, 9};
    double expectedData[] = {9, 7};
    Matrix actual, expected;
    Vector actualV, expectedV;

    ml_createCostTestVector(&actualV, actualData, 2);
    ml_createCostTestMatrix(&actual, &actualV);
    ml_createCostTestVector(&expectedV, expectedData, 2);
    ml_createCostTestMatrix(&expected, &expectedV);

    const double actualCost = getCost(&actual, &expected);

    printf("actual cost: %lf, expected cost: %lf\n", actualCost, expectedCost);
    assert(actualCost == expectedCost);
    return actualCost == expectedCost; 
}

bool ml_testAverageCost() {
    printf("[TEST] %s, %d\n", __FILE__, __LINE__);
    const double expectedCost = 13.25;
    double actualData1[] = {6, 9};
    double actualData2[] = {7, 9};
    double expectedData1[] = {9, 7};
    double expectedData2[] = {5, 3};
    Matrix actual1, actual2;
    Matrix expected1, expected2;
    Vector actual1V, actual2V;
    Vector expected1V, expected2V;

    ml_createCostTestVector(&actual1V, actualData1, 2);
    ml_createCostTestMatrix(&actual1, &actual1V);
    ml_createCostTestVector(&expected1V, expectedData1, 2);
    ml_createCostTestMatrix(&expected1, &expected1V);
    ml_createCostTestVector(&actual2V, actualData2, 2);
    ml_createCostTestMatrix(&actual2, &actual2V);
    ml_createCostTestVector(&expected2V, expectedData2, 2);
    ml_createCostTestMatrix(&expected2, &expected2V);

    Matrix* actuals[] = {&actual1, &actual2};  
    Matrix* expecteds[] = {&expected1, &expected2};

    const double actualCost = getAverageCost(actuals, expecteds, 2);

    printf("actual cost: %lf, expected cost: %lf\n", actualCost, expectedCost);
    assert(actualCost == expectedCost);
    return actualCost == expectedCost;
}

bool ml_testSGD() {
    printf("[TEST] %s, %d\n", __FILE__, __LINE__);

    NeuralNet testNet;
    LayerParams layerParams[3] = {
        {3, _BUILTIN_NONE},
        {4, _BUILTIN_RELU},
        {2, _BUILTIN_RELU}
    };

    assert(createNeuralNet(&testNet, layerParams, 3));

    const char* testFilename = "learning/tests/test_data/sgd.test";
    FILE* file = fopen(testFilename, "r");
    assert(file != NULL);

    uint8 weightedLayerCount;
    fscanf(file, "%d", &weightedLayerCount);

    for (uint8 i = 0; i < weightedLayerCount; i++) {
        HiddenLayer* layer = NULL;
        if (i != weightedLayerCount-1) {
            layer = &testNet.hiddenLayers[i];
        }
        else {
            layer = &testNet.outputLayer;
        }

        Dimensions2D weightDimensions;
        fscanf(file, "%llu %llu", &weightDimensions.rows, &weightDimensions.cols);

        for (size_t row = 0; row < weightDimensions.rows; row++) {
            for (size_t col = 0; col < weightDimensions.cols; col++) {
                double value;
                fscanf(file, "%lf", &value);
                assert(setMatrixElement(&layer->weights, row, col, value));
            }
        }

        Dimensions2D biasDimensions;
        fscanf(file, "%llu %llu", &biasDimensions.rows, &biasDimensions.cols);

        for (size_t row = 0; row < biasDimensions.rows; row++) {
            for (size_t col = 0; col < biasDimensions.cols; col++) {
                double value;
                fscanf(file, "%lf", &value);
                assert(setMatrixElement(&layer->biases, row, col, value));
            }
        }
    }

    Vector input;
    assert(createVector(&input, 3));
    input.data[0] = 1;
    input.data[1] = 1;
    input.data[2] = 1;
    assert(modelPredict(&testNet, &input));

    Matrix expectedPrediction;
    assert(createMatrix(&expectedPrediction, 2, 1));
    assert(setMatrixElement(&expectedPrediction, 0, 0, 21));
    assert(setMatrixElement(&expectedPrediction, 1, 0, 22));

    assert(areEqualMatrices(&expectedPrediction, &testNet.outputLayer.activationOutputs));

    // set values to be not same as prediction then run optimizer
    assert(setMatrixElement(&expectedPrediction, 0, 0, 20));
    assert(setMatrixElement(&expectedPrediction, 1, 0, 23));
    
    LayersUpdates updates;
    assert(createLayersUpdates(&updates, &testNet));
    assert(gradientDescent(&testNet, &expectedPrediction, &updates));
    assert(updateParameters(&testNet, &updates, 1));

    bool success = true;
    for (uint8 i = 0; i < weightedLayerCount; i++) {
        HiddenLayer* layer = NULL;
        if (i != weightedLayerCount-1) {
            layer = &testNet.hiddenLayers[i];
        }
        else {
            layer = &testNet.outputLayer;
        }

        // weights check
        size_t rows = 0;
        size_t cols = 0;
        fscanf(file, "%llu %llu", &rows, &cols);
        Matrix temp;
        assert(createMatrix(&temp, rows, cols));

        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                double value;
                fscanf(file, "%lf", &value);
                assert(setMatrixElement(&temp, row, col, value));
            }
        }

        printf("Weight comparison:\n");
        displayMatrix(&temp);
        printf("\n");
        displayMatrix(&layer->weights);
        printf("\n");
        assert(areEqualMatrices(&temp, &layer->weights));
        if (!areEqualMatrices(&temp, &layer->weights)) {
            success = false;
            assert(success);
        }

        deleteMatrix(&temp);

        // biases check
        fscanf(file, "%llu %llu", &rows, &cols);
        assert(createMatrix(&temp, rows, cols));

        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                double value;
                fscanf(file, "%lf", &value);
                assert(setMatrixElement(&temp, row, col, value));
            }
        }

        printf("Biases comparison:\n");
        displayMatrix(&temp);
        printf("\n");
        displayMatrix(&layer->biases);
        printf("\n");
        assert(areEqualMatrices(&temp, &layer->biases));
        if (!areEqualMatrices(&temp, &layer->biases)) {
            success = false;
            assert(success);
        }
    }

    return success;
}