#ifndef MALLOC_DEFINITIONS_H
#define MALLOC_DEFINITIONS_H
#include <stddef.h>
// -- COMPILER CHECKED INTERNAL LOCK --
// Using lock with this definition ensures that lock and unlock are
// always performed internally in the function. If UNLOCK_INTERNAL is not
// performed after LOCK_INTERNAL, a compiler error will be raised.
// for type definition
#include <pthread.h>
#include <stdint.h>
// #define DEBUG
typedef uint8_t BYTE;
// visability keyword setting
#define __OPEN_API__ __attribute__((visibility("default")))
#define __INTERNAL__ __attribute__((visibility("hidden")))
#define __CONSTRUCTOR__ __attribute__((constructor))
#define __DESTRUCTOR__ __attribute__((destructor))

#endif