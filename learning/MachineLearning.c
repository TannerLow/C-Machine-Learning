#include "MachineLearning.h"
#include "../matrix/MatrixMath.h"
#include <math.h>
#include <assert.h>

double getCost(Matrix* actual, Matrix* expected) {
    assert(actual != NULL && expected != NULL);

    if (actual == NULL || expected == NULL);

    double cost = 0;
    // cost = summation[ (e[i] - a[i])^2 ] / n
    if (matrixSubtraction(expected, actual, expected)) {
        for (size_t col = 0; col < expected->rowSize; col++) {
            for (size_t row = 0; row < expected->columnSize; row++) {
                cost += pow(2, getMatrixElement(expected, row, col));
            }
        }
        cost /= expected->rowSize * expected->columnSize;
    }

    return cost;
}

double getAverageCost(Matrix** actuals, Matrix* expecteds) {

}