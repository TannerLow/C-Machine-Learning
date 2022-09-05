#include "stdsafety.h"
#include <stdlib.h>


#ifdef DEBUG // debug mode

#include <stdio.h>
void* _safeMalloc(const size_t bytes, const size_t line, const char* file) {
    void* result = malloc(bytes);
    if (result == NULL) {
        printf("!!! === [safeMalloc failed, abort called](Line: %llu, File: %s) === !!!\n", line, file);
        abort();
    }
    return result;
}

#else // non-debug mode

void* _safeMalloc(const size_t bytes) {
    void* result = malloc(bytes);
    if (result == NULL) {
        abort();
    }
    return result;
}

#endif // DEBUG 