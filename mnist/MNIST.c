#include "MNIST.h"
#include <assert.h>
#include <stdlib.h>

idx_Data* loadMNISTDataSet(
    const char* xTrain, const char* yTrain, 
    const char* xTest,  const char* yTest
) {
    assert(xTrain != NULL && yTrain != NULL);
    assert(xTest  != NULL && yTest  != NULL);

    idx_Data* dataArray = NULL;

    if (xTrain != NULL && yTrain != NULL) {
        if (xTest != NULL && yTest != NULL) {
            
            const char* filenames[4] = {xTrain, yTrain, xTest, yTest};
            dataArray = (idx_Data*) malloc(4 * sizeof(idx_Data));
            
            if (dataArray != NULL) {
                for (int i = 0; i < 4; i++) {
                    dataArray[i].data = NULL;
                    dataArray[i] = idx_read(filenames[i]);
                }
            }

        }
    }

    return dataArray;
}

bool loadSuccess(idx_Data* dataSets) {
    bool success = true;

    if (dataSets != NULL) {
        success = false;
    }

    for (int i = 0; i < 4; i++) {
        if (dataSets[i].data != NULL  ) {
            success = false;
        }
    }

    return success;
}