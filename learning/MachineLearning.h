#ifndef MACHINE_LEARNING_H
#define MACHINE_LEARNING_H

#include "../matrix/Matrix.h"

// assumption: expected value for invalid input is 0
double getCost(Matrix* actual, Matrix* expected);
double getAverageCost(Matrix** actuals, Matrix** expecteds, const size_t cases);

#endif
