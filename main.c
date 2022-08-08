#include <stdio.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "model/NeuralNet.h"

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


    return 0;
}