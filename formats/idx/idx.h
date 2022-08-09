#ifndef IDX_H
#define IDX_H

#include <stddef.h>

// For info on IDX file format, see this website
// http://yann.lecun.com/exdb/mnist/

#define IDX_ERROR  0x00
#define IDX_UBYTE  0x08
#define IDX_BYTE   0x09
#define IDX_SHORT  0x0B
#define IDX_INT    0x0C
#define IDX_FLOAT  0x0D
#define IDX_DOUBLE 0x0E

typedef unsigned char ubyte;
typedef ubyte IdxTypeEnum;

typedef struct {
    void* data;
    size_t size;
    int* dimensionSizes;
    ubyte dimensions;
    IdxTypeEnum type;
} idx_Data;

idx_Data idx_read(const char* filename);
ubyte idx_sizeOfElement(const IdxTypeEnum type);
void idx_freeData(idx_Data* data);

// TODO - move this to a better suited file eventually
#include <stdbool.h>
#define BIG_ENDIAN 1
#define LITTLE_ENDIAN 0
typedef bool ENDIANESS;
ENDIANESS determineCPUEndianess();
// clobbers input data array
int convertEndianessInt(unsigned char* data);

#endif // IDX_H