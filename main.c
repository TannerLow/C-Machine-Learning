#include <stdio.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "tests/tester.h"
#include "NeuralNet.h"

int main() {

    #ifdef RUN_TESTS
        test();
    #endif

    NeuralNet nn;
    LayerParams layerParams[] = {
        {10, relu},
        {9, relu},
        {5, relu},
        {2, softmax},
    };

    if (!createNeuralNet(&nn, layerParams, 4)) {
        printf("Failed to create Neural Net\n");
    }

    deleteNeuralNet(&nn);

    return 0;
}