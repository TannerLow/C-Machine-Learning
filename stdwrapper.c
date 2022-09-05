#include "stdwrapper.h"
#include <assert.h>

void* safeMalloc(const size_t bytes) {
    void* result = malloc(bytes);
    assert(result != NULL);
    if (result == NULL) {
        abort();
    }
    return result;
}