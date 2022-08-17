#include "MNIST.h"
#include <assert.h>
#include <stdlib.h>

MNIST_Data loadMNISTDataSet(
    const char* xTrain, const char* yTrain, 
    const char* xTest,  const char* yTest
) {
    assert(xTrain != NULL && yTrain != NULL);
    assert(xTest  != NULL && yTest  != NULL);

    MNIST_Data data;

    if (xTrain != NULL && yTrain != NULL) {
        if (xTest != NULL && yTest != NULL) {

            idx_Data* dataSets[4] = {
                &data.xTrain, &data.yTrain, 
                &data.xTest,  &data.yTest
            };

            const char* filenames[4] = {
                xTrain, yTrain, 
                xTest,  yTest
            };

            for (int i = 0; i < 4; i++) {
                dataSets[i]->data = NULL;
                *dataSets[i] = idx_read(filenames[i]);
            }

        }
    }

    return data;
}

void freeMNISTData(MNIST_Data* dataset) {
    idx_freeData(&dataset->xTrain);
    idx_freeData(&dataset->yTrain);
    idx_freeData(&dataset->xTest);
    idx_freeData(&dataset->yTest);
}

bool loadSuccess(MNIST_Data* dataSet) {
    return dataSet->xTrain.data != NULL && dataSet->yTrain.data != NULL
        && dataSet->xTest.data  != NULL && dataSet->yTest.data  != NULL;
}