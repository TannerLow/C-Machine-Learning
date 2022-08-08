#include "MachineLearning.h"
#include "../matrix/MatrixMath.h"
#include <math.h>
#include <assert.h>

// NOTE: can const qualify if I make an in-place subtraction function
double getCost(Matrix* actual, Matrix* expected) {
    assert(actual != NULL && expected != NULL);

    double cost = 0;
    if (actual != NULL && expected != NULL) {
        Matrix result;

        // cost = summation[ (e[i] - a[i])^2 ]
        if (createMatrix(&result, actual->columnSize, actual->rowSize) && matrixSubtraction(expected, actual, &result)) {
            for (size_t col = 0; col < result.rowSize; col++) {
                for (size_t row = 0; row < result.columnSize; row++) {
                    cost += pow(getMatrixElement(&result, row, col), 2);
                }
            }
        }

    }

    return cost;
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