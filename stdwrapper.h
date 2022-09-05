#ifndef STD_WRAPPER
#define STD_WRAPPER

#include <stdlib.h>

// malloc wrapper that asserts in debug
// and aborts the program if malloc fails
void* safeMalloc(const size_t bytes);

#endif // STD_WRAPPER