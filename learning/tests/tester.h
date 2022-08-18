#ifndef ML_TESTER_H
#define ML_TESTER_H

#include <stdbool.h>

// assumption: running in test_debug configuration
bool ml_testAll();
bool ml_testCost();
bool ml_testAverageCost();
bool ml_testSGD();

#endif // ML_TESTER_H