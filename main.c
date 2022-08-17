#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "matrix/Matrix.h"
#include "matrix/MatrixMath.h"
#include "model/NeuralNet.h"
#include "model/Activations.h"
#include "formats/idx/idx.h"
#include "mnist/MNIST.h"
#include "matrix/MatrixDebug.h"
#include "learning/MachineLearning.h"

#ifdef RUN_TESTS
#include "matrix/tests/tester.h"
#include "model/tests/tester.h"
#include "learning/tests/tester.h"
#endif

void testWithTensorFlowModel();

int main() {

    #ifdef RUN_TESTS
        matrix_testAll();
        model_testAll();
        ml_testAll();
    #endif

    // idx_Data xTrain = idx_read("data/train-images.idx3-ubyte");
    // // prints a rough looking 5 as proof of concept
    // for (ubyte row = 0; row < 28; row++) {
    //     for (ubyte col = 0; col < 28; col++) {
    //         printf("%d ", *((ubyte*)xTrain.data + row * 28 + col) > 0 ? 1 : 0);
    //     }
    //     printf("\n");
    // }

    // idx_freeData(&xTrain);

    MNIST_Data data = loadMNISTDataSet(
        "data/train-images.idx3-ubyte", "data/train-labels.idx1-ubyte",
        "data/t10k-images.idx3-ubyte", "data/t10k-labels.idx1-ubyte"
    );

    printf("MNIST loaded: %d\n", loadSuccess(&data));

    freeMNISTData(&data);

    NeuralNet model;

    LayerParams layerParams[3] = {
        // format: {layer size, {activation function, derivative, optimizable?}}
        {3 , {NULL,    NULL,     false}},
        {15, {relu,    dRelu,    true}},
        {4 , {softmax, dSoftmax, false}}
    };

    createNeuralNet(&model, layerParams, 3);

    randomizeWeights(&model);
    randomizeBiases(&model);

    printf("weights:\n");
    displayMatrix(&model.hiddenLayers[0].weights); 

    printf("biases:\n");
    displayMatrix(&model.hiddenLayers[0].biases);

    Vector vector;
    createVector(&vector, 3);
    vector.data[0] = 0;
    vector.data[1] = 1;
    vector.data[2] = 0.5;
    modelPredict(&model, &vector);

    printf("prediction certainties:\n");
    displayMatrix(&model.outputLayer.activationOutputs);

    Matrix expected;
    createMatrix(&expected, model.outputLayer.size, 1);
    Vector expectedV;
    createVector(&expectedV, 4);
    vector.data[0] = 0;
    vector.data[1] = 1;
    vector.data[2] = 0;
    vector.data[3] = 0;
    expected.columns[0] = expectedV;
    optimizeSGD(&model, &expected, 0.1);

    deleteNeuralNet(&model);

    //testWithTensorFlowModel();

    return 0;
}

// helper for testWithTensorFlowModel
// double* loadModelDatFileData(const char* filename, const size_t size) {
//     assert(filename);

//     double* data = NULL;
//     float* dataFloats = NULL;
//     if (filename == NULL) return data;

//     FILE* file = fopen(filename, "rb");
//     if (file == NULL) return data;

//     data = (double*) malloc(size * sizeof(double));
//     if (data == NULL) return data;

//     dataFloats = (float*) malloc(size * sizeof(float));
//     if (dataFloats == NULL) {
//         free(data);
//         data = NULL;
//         return data;
//     }

//     fread((void*)dataFloats, sizeof(float), size, file);

//     for (size_t i = 0; i < size; i++) {
//         data[i] = (double) dataFloats[i];
//     }

//     fclose(file);
//     free(dataFloats);
//     dataFloats = NULL;
//     return data;
// }

// void testWithTensorFlowModel() {
//     idx_Data xTrain = {.data = NULL};

//     //     size = ( L1 +  L0 *  L1) + ( L2 +  L1 *  L2) + (L3 +  L2 * L3)
//     size_t size = (500 + 784 * 500) + (500 + 500 * 500) + (10 + 500 * 10);
//     size_t segmentLengths[] = { 
//         500, 784*500, 
//         500, 500*500, 
//         10, 500*10 
//     };

//     double* data = loadModelDatFileData("model.dat", size);

//     NeuralNet model;
//     LayerParams layerParams[4] = {
//         {784, 0},
//         {500, relu},
//         {500, relu},
//         {10, softmax}
//     };
//     if (!createNeuralNet(&model, layerParams, 4)) {
//         printf("Failed to create model (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

// // set all the weights and biases of the model

//     int i = 0;
//     size_t offset = 0;
//     if (!setBiases(&model.hiddenLayers[0], data + offset))  {
//         printf("Failed to load biases (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     offset += segmentLengths[i++];
//     if (!setWeights(&model.hiddenLayers[0], data + offset)) {
//         printf("Failed to load weights (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     offset += segmentLengths[i++];
//     if (!setBiases(&model.hiddenLayers[1], data + offset))  {
//         printf("Failed to load biases (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     offset += segmentLengths[i++];
//     if (!setWeights(&model.hiddenLayers[1], data + offset)) {
//         printf("Failed to load biases (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     offset += segmentLengths[i++];
//     if (!setBiases(&model.outputLayer, data + offset))  {
//         printf("Failed to load biases (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     offset += segmentLengths[i++];
//     if (!setWeights(&model.outputLayer, data + offset)) {
//         printf("Failed to load biases (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

// // setup input and predict

//     xTrain = idx_read("data/train-images.idx3-ubyte");
//     if (xTrain.data == NULL) {
//         printf("Faild to load input data from file (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     Vector input;
//     if (!createVector(&input, 28*28)) {
//         printf("Faild to allocate memory for input vector (%s, %d)\n", __FILE__, __LINE__);
//         goto CLEAN_AND_EXIT;
//     }

//     //image of a 5
//     for (ubyte row = 0; row < 28; row++) {
//         for (ubyte col = 0; col < 28; col++) {
//             // printf("%d ", *((ubyte*)xTrain.data + row * 28 + col) > 0 ? 1 : 0);
//             input.data[row * 28 + col] = *((ubyte*)xTrain.data + row * 28 + col) / 255.0;
//             //printf("%lf\n", input.data[row * 28 + col]);
//         }
//         //printf("\n");
//     }

//     for (size_t j = 0; j < input.size; j++) {
//         if (input.data[j] != 0) {
//             printf("first non-zero input: %lf %llu\n", input.data[j], j);
//             break;
//         }
//     }

//     idx_freeData(&xTrain);
//     //goto CLEAN_AND_EXIT;

//     printf("b: %lf\n", getMatrixElement(&model.hiddenLayers[0].biases, 10, 0));
//     printf("w: %lf\n", getMatrixElement(&model.hiddenLayers[0].weights, 0, 10));
//     printf("b: %lf\n", getMatrixElement(&model.hiddenLayers[1].biases, 10, 0));
//     printf("w: %lf\n", getMatrixElement(&model.hiddenLayers[1].weights, 0, 10));
//     printf("b: %lf\n", getMatrixElement(&model.outputLayer.biases, 5, 0));
//     printf("w: %lf\n", getMatrixElement(&model.outputLayer.weights, 0, 10));
//     printf("input sample: %lf\n", input.data[8*28 + 7]);

//     for (size_t j = 500; j < 1000; j++) {
//         printf("%lf, ", getMatrixElement(&model.hiddenLayers[1].weights, j / 500, j % 500));
//     }
//      printf("\n");

//     if(!modelPredict(&model, &input)) {
//         goto CLEAN_AND_EXIT;
//     }
//     for (size_t j = 0; j < 10; j++) {
//         printf("%llu: %lf\n", j, getMatrixElement(&model.outputLayer.activationOutputs, j, 0));
//     }

//     printf("Prediction: %llu\n", maxIndex(&model.outputLayer.activationOutputs.columns[0]));

// CLEAN_AND_EXIT:
//     free(data);
//     deleteNeuralNet(&model);
//     idx_freeData(&xTrain);
// }