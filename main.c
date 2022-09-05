#include "logger/logger.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "model/NeuralNet.h"
#include "model/Activations.h"
#include "formats/idx/idx.h"
#include "mnist/MNIST.h"
#include "matrix/MatrixDebug.h"
#include "learning/MachineLearning.h"
#include <errno.h>

#ifdef RUN_TESTS
#include "matrix/tests/tester.h"
#include "model/tests/tester.h"
#include "learning/tests/tester.h"
#endif

// global variable initialization
Logger global_logger;

void testWithTensorFlowModel();
void setInput(Matrix* input, ubyte* data, size_t index);
void setExpected(Matrix* expected, uint8 digit);
bool isCorrect(Vector* actual, Vector* expected);


int main() {
    assert(logger_create(&global_logger, "log.txt"));

    #ifdef RUN_TESTS
        matrix_testAll();
        model_testAll();
        ml_testAll();
    #endif

    logger_close(&global_logger);

    MNIST_Data data = loadMNISTDataSet(
        "data/train-images.idx3-ubyte", "data/train-labels.idx1-ubyte",
        "data/t10k-images.idx3-ubyte", "data/t10k-labels.idx1-ubyte"
    );

    printf("\nMNIST loaded: %d\n", loadSuccess(&data));

    NeuralNet model;
    LayerParams layerParams[3] = {
        {784, _BUILTIN_NONE},
        {500, _BUILTIN_RELU},
        //{500, _BUILTIN_RELU},
        {10, _BUILTIN_SOFTMAX}
    };

    assert(createNeuralNet(&model, layerParams, 3));
    assert(randomizeWeights(&model));
    assert(randomizeBiases(&model));

    NeuralNet deltaNet;
    assert(copyNetworkStructure(&model, &deltaNet));

    const size_t cases = data.xTrain.size / 784;
    const size_t batchSize = 50;

    Matrix input, expected;
    assert(createMatrix(&input, 784, 1));
    assert(createMatrix(&expected, 10, 1));
    printf("Training on %llu cases\n", cases);

    int correct = 0;
    LayersUpdates updatesPool;
    LayersUpdates localUpdates;
    assert(createLayersUpdates(&localUpdates, &model));
    assert(createLayersUpdates(&updatesPool, &model));
    assert(clearLayersUpdates(&updatesPool));

    size_t sample = 0;
    for (size_t i = 0; i < cases; i++) {
        if ((sample++ != 0) && (sample % batchSize == 0)) {
            printf("Progress: %llu/%llu\n", sample, cases);
            printf("Acurracy: %lf\n", (double) correct / (double) i);
            assert(updateParameters(&model, &updatesPool, 0.5 / (double) batchSize));
            assert(clearLayersUpdates(&updatesPool));
        }

        setInput(&input, (ubyte*)data.xTrain.data, i);
        setExpected(&expected, ((ubyte*)data.yTrain.data)[i]);

        assert(modelPredict(&model, &input.columns[0]));
        if (isCorrect(&model.outputLayer.activationOutputs.columns[0], &expected.columns[0])) correct++;
        assert(gradientDescent(&model, &expected, &localUpdates, &deltaNet));
        assert(addLayersUpdates(&localUpdates, &updatesPool, &updatesPool));
    }
    
    printf("Acurracy: %lf\n", (double) correct / (double) cases);
    printf("Validation on a 5 image:\n");
    setInput(&input, (ubyte*)data.xTrain.data, 0);
    setExpected(&expected, ((ubyte*)data.yTrain.data)[0]);

    printf("expected:\n");
    displayMatrix(&expected);

    modelPredict(&model, &input.columns[0]);
    printf("prediction:\n");
    displayMatrix(&model.outputLayer.activationOutputs);

    printf("\nLast layer Z values\n");
    displayMatrix(&model.outputLayer.activationInputs);
    
    // clean up
    deleteMatrix(&input);
    deleteMatrix(&expected);
    deleteNeuralNet(&model);
    deleteLayersUpdates(&localUpdates);
    deleteLayersUpdates(&updatesPool);

    freeMNISTData(&data);

    return 0;
}

void setInput(Matrix* input, ubyte* data, size_t index) {
    assert(input != NULL);
    assert(data != NULL);

    for (size_t i = 0; i < 28*28; i++) {
        assert(setMatrixElement(input, i, 0, (double)(data[index * 28*28 + i]) / 255.0));
    }
}

void setExpected(Matrix* expected, uint8 digit) {
    assert(expected != NULL);

    for (size_t i = 0; i < expected->columnSize; i++) {
        assert(setMatrixElement(expected, i, 0, (i == digit ? 1.0 : 0.0)));
    }
}

bool isCorrect(Vector* actual, Vector* expected) {
    double max = 0;
    int index = -1;
    for (size_t i = 0; i < actual->size; i++) {
        if (actual->data[i] > max) {
            max = actual->data[i];
            index = i;
        }
    }

    if (index >= 0) {
        return expected->data[index] == 1.0;
    }
    return false;
}