#include "tester.h"
#include "../MachineLearning.h"
#include "../../matrix/Matrix.h"
#include <stdio.h>
#include <assert.h>

bool ml_testAll() {
    bool cost = ml_testCost();
    bool averageCost = ml_testAverageCost();

    printf("Cost Test: %s\n", cost ? "success" : "FAILURE");
    printf("Cost Test: %s\n", averageCost ? "success" : "FAILURE");
    printf("\n");

    return cost && averageCost;
}

void ml_createCostTestVector(Vector* vector, double* data, const size_t size) {
    assert(vector != NULL);
    assert(data != NULL);
    assert(size != 0);

    vector->data = data;
    vector->size = size;
}

void ml_createCostTestMatrix(Matrix* matrix, Vector* data) {
    assert(matrix != NULL);
    assert(data != NULL);

    matrix->columns = data;
    matrix->columnSize = data->size;
    matrix->rowSize = 1;
}

bool ml_testCost() {
    printf("[TEST] %s, %d\n", __FILE__, __LINE__);
    const double expectedCost = 6.5;
    double actualData[] = {6, 9};
    double expectedData[] = {9, 7};
    Matrix actual, expected;
    Vector actualV, expectedV;

    ml_createCostTestVector(&actualV, actualData, 2);
    ml_createCostTestMatrix(&actual, &actualV);
    ml_createCostTestVector(&expectedV, expectedData, 2);
    ml_createCostTestMatrix(&expected, &expectedV);

    const double actualCost = getCost(&actual, &expected);

    printf("actual cost: %lf, expected cost: %lf\n", actualCost, expectedCost);
    assert(actualCost == expectedCost);
    return actualCost == expectedCost; 
}

bool ml_testAverageCost() {
    printf("[TEST] %s, %d\n", __FILE__, __LINE__);
    const double expectedCost = 13.25;
    double actualData1[] = {6, 9};
    double actualData2[] = {7, 9};
    double expectedData1[] = {9, 7};
    double expectedData2[] = {5, 3};
    Matrix actual1, actual2;
    Matrix expected1, expected2;
    Vector actual1V, actual2V;
    Vector expected1V, expected2V;

    ml_createCostTestVector(&actual1V, actualData1, 2);
    ml_createCostTestMatrix(&actual1, &actual1V);
    ml_createCostTestVector(&expected1V, expectedData1, 2);
    ml_createCostTestMatrix(&expected1, &expected1V);
    ml_createCostTestVector(&actual2V, actualData2, 2);
    ml_createCostTestMatrix(&actual2, &actual2V);
    ml_createCostTestVector(&expected2V, expectedData2, 2);
    ml_createCostTestMatrix(&expected2, &expected2V);

    Matrix* actuals[] = {&actual1, &actual2};  
    Matrix* expecteds[] = {&expected1, &expected2};

    const double actualCost = getAverageCost(actuals, expecteds, 2);

    printf("actual cost: %lf, expected cost: %lf\n", actualCost, expectedCost);
    assert(actualCost == expectedCost);
    return actualCost == expectedCost;
}