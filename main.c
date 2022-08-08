#include <stdio.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "model/NeuralNet.h"

#ifdef RUN_TESTS
#include "matrix/tests/tester.h"
#include "model/tests/tester.h"
#endif


int main() {

    #ifdef RUN_TESTS
        test();
        model_testAll();
    #endif


    return 0;
}