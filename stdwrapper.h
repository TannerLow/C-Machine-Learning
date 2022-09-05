#ifndef STD_WRAPPER
#define STD_WRAPPER

#include <stdlib.h>

// malloc wrapper that asserts in debug
// and aborts the program if malloc fails
#ifdef DEBUG
void* _safeMalloc(const size_t bytes, const size_t line, const char* file);
#define safeMalloc(in) _safeMalloc((in), __LINE__, __FILE__)
#else
void* _safeMalloc(const size_t bytes);
#define safeMalloc(in) _safeMalloc((in))
#endif


#endif // STD_WRAPPER