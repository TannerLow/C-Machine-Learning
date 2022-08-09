#include <stdio.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "model/NeuralNet.h"

#include "formats/idx/idx.h"

#ifdef RUN_TESTS
#include "matrix/tests/tester.h"
#include "model/tests/tester.h"
#include "learning/tests/tester.h"
#endif


int main() {

    #ifdef RUN_TESTS
        matrix_testAll();
        model_testAll();
        ml_testAll();
    #endif

    idx_Data xTrain = idx_read("train-images.idx3-ubyte");
    // prints a rough looking 5 as proof of concept
    for (ubyte row = 0; row < 28; row++) {
        for (ubyte col = 0; col < 28; col++) {
            printf("%d ", *((ubyte*)xTrain.data + row * 28 + col) > 0 ? 1 : 0);
        }
        printf("\n");
    }

    idx_freeData(&xTrain);

    return 0;
}