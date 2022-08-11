#include "idx.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
    #define IDX_LOG(x) x
#else
    void idx_noOp() {}
    #define IDX_LOG(x) idx_noOp()
#endif


idx_Data idx_read(const char* filename) {
    assert(filename != NULL);

    idx_Data fileData = {
        .data           = NULL,
        .size           = 0,
        .dimensionSizes = NULL,
        .dimensions     = 0,
        .type           = IDX_ERROR
    };

    ENDIANESS cpuEndianess = determineCPUEndianess();

    FILE* file = fopen(filename, "rb");
    assert(file != NULL);
    if (file == NULL) return fileData; // unable to open file

    // TODO - get size of file then compare after getting magic number and dims

// get magic number which gives info about file structure
    ubyte magicNumber[4];
    memset(&magicNumber, 0, 4); // clear buffer
    fread(magicNumber, 1, 4, file);

    IDX_LOG(printf("Magic number bytes: 0x%X 0x%X 0x%X 0x%X\n", magicNumber[0], magicNumber[1], magicNumber[2], magicNumber[3]));

    fileData.type = magicNumber[2];
    fileData.dimensions = magicNumber[3];

    fileData.dimensionSizes = (int*) malloc(fileData.dimensions * sizeof(int));
    if (fileData.dimensionSizes == NULL) return fileData; // unable to allocate memory for dim sizes    

    size_t totalDataBytes = 1;
    ubyte buffer[4];
    for (ubyte i = 0; i < fileData.dimensions; i++) {
        fread(buffer, 1, 4, file);
        int n = *((int*) buffer);
        if (cpuEndianess == LITTLE_ENDIAN) {
            n = convertEndianessInt(buffer);
        }
        fileData.dimensionSizes[i] = n;
        totalDataBytes *= n;
    }
    fileData.size = totalDataBytes;
    totalDataBytes *= idx_sizeOfElement(fileData.type);
    IDX_LOG(printf("data  bytes count: %lld\n", fileData.size));
    IDX_LOG(printf("total bytes count: %lld\n", totalDataBytes));

    IDX_LOG(for (ubyte i = 0; i < fileData.dimensions; i++) {
        printf("size of dimension %d: %d\n", i, fileData.dimensionSizes[i]);
    });

    fileData.data = malloc(totalDataBytes);

    if (fileData.type == IDX_UBYTE || fileData.type == IDX_BYTE) {
        fread(fileData.data, 1, totalDataBytes, file);
    }

    fclose(file);

    return fileData; 
}

ubyte idx_sizeOfElement(const IdxTypeEnum type) {
    switch (type) {
        case IDX_UBYTE  : return 1;
        case IDX_BYTE   : return 1;
        case IDX_SHORT  : return 2;
        case IDX_INT    : return 4;
        case IDX_FLOAT  : return 4;
        case IDX_DOUBLE : return 8;
        default         : return 0;
    }
}

void idx_freeData(idx_Data* data) {
    assert(data != NULL);

    if (data != NULL) {
        free(data->data);
        free(data->dimensionSizes);
        data->data = NULL;
        data->dimensionSizes = NULL;
    }
}

ENDIANESS determineCPUEndianess() {
    char arr[] = {1,0};
    short s = *((short*) arr);
    return s == 1 ? LITTLE_ENDIAN : BIG_ENDIAN;
}

int convertEndianessInt(unsigned char* data) {
    assert(data != NULL);
    unsigned char temp = data[0];
    data[0] = data[3];
    data[3] = temp;
    temp = data[1];
    data[1] = data[2];
    data[2] = temp;
    return *((int*) data);
}