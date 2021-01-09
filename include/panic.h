#ifndef __PANIC_H_
#define __PANIC_H_

#include "stdio.h"

#define PANIC(...) do { \
    printf("PANIC! "); \
    printf(__VA_ARGS__); \
    while(1) {};\
} while(0);

#define WARN(...) do { \
    printf("Warning: "); \
    printf(__VA_ARGS__); \
} while(0);

#endif // __PANIC_H_
