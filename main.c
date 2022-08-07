#include <stdio.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "tests/tester.h"
#include "NeuralNet.h"
#include "MachineLearning.h"

int main() {

    #ifdef RUN_TESTS
        test();
    #endif

    NeuralNet nn;
    LayerParams layerParams[] = {
        {5, relu},
        {9, relu},
        {5, relu},
        {2, softmax},
    };

    if (!createNeuralNet(&nn, layerParams, 4)) {
        printf("Failed to create Neural Net\n");
        return 1;
    }

    Vector input;
    if (!createVector(&input, 5)) {
        printf("Failed to craet input vector\n");
        return 2;
    }

    if (!predict(&nn, &input)) {
        printf("Failed prediction\n");
        return 3;
    }

    printf("Prediction succeeded\n");

    deleteVector(&input);
    deleteNeuralNet(&nn);

    return 0;
}