#ifndef MNIST_H
#define MNIST_H

#include <stdbool.h>
#include "../formats/idx/idx.h"

idx_Data* loadMNISTDataSet(
    const char* xTrain, const char* yTrain, 
    const char* xTest, const char* yTest
);

bool loadSuccess(idx_Data* dataSets);


#endif // MNIST_H