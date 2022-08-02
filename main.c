#include <stdio.h>
#include "Matrix.h"
#include "MatrixMath.h"
#include "tester.h"

void test();

int main() {
    test();

    return 0;
}

// void test() {

//     Vector v;
//     createVector(&v, 10);
//     printf("Test vector size after create: %d\n", v.size);
//     deleteVector(&v);
//     printf("Test vector size after delete: %d\n", v.size);

//     Matrix m;
//     createMatrix(&m, 15, 15);
//     printf("Test matrix size after create: %d x %d\n", m.rowSize, m.columnSize);
//     deleteMatrix(&m);
//     printf("Test matrix size after create: %d x %d\n", m.rowSize, m.columnSize);

//     Matrix a;
//     Matrix b;
//     Matrix c;
    
//     if (!createMatrix(&a, 3, 2)) {
//         printf("Failed to create matrix: main.c,%d\n", __LINE__);
//     }

//     if (!createMatrix(&b, 2, 3)) {
//         printf("Failed to create matrix: main.c,%d\n", __LINE__);
//     }

//     if (!createMatrix(&c, 3, 4)) {
//         printf("Failed to create matrix: main.c,%d\n", __LINE__);
//     }

//     if(!dotProduct(&a, &b, &c)) {
//         printf("Failed to perform dot product: main.c,%d\n", __LINE__);
//     }

//     displayMatrix(&c);

//     printf("Done Testing\n");
// }