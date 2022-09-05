#ifndef STD_SAFETY
#define STD_SAFETY

#include <stdbool.h>


/* Below is macro-specified signatures of functions
 * to provide different functionality in debug mode */
#ifdef DEBUG // debug mode

    #include <assert.h>

    // malloc wrapper, prints debug info and aborts if failure
    void* _safeMalloc(const size_t bytes, const size_t line, const char* file);
    #define safeMalloc(_in) _safeMalloc((_in), __LINE__, __FILE__)

    // assert+ functions
    #define assertOrReturnFalse(_expression) assert((_expression))
    #define assertOrAbort(_expression) assert((_expression))

#else // non-debug mode

    // malloc wrapper, aborts if failure
    void* _safeMalloc(const size_t bytes);
    #define safeMalloc(_in) _safeMalloc((_in))

    // assert+ functions
    #define assertOrReturnFalse(_expression) if (!(_expression)) return false
    #define assertOrAbort(_expression) if (!(_expression)) abort()

#endif // DEBUG

#endif // STD_SAFETY