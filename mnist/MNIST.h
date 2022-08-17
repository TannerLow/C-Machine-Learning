#ifndef MNIST_H
#define MNIST_H

#include <stdbool.h>
#include "../formats/idx/idx.h"

typedef struct {
    idx_Data xTrain;
    idx_Data yTrain;
    idx_Data xTest;
    idx_Data yTest;
} MNIST_Data;

MNIST_Data loadMNISTDataSet(
    const char* xTrain, const char* yTrain, 
    const char* xTest, const char* yTest
);

void freeMNISTData(MNIST_Data* dataset);

bool loadSuccess(MNIST_Data* dataSets);


#endif // MNIST_H