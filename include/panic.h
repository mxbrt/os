#ifndef __PANIC_H_
#define __PANIC_H_

#include <dbgu.h>

#define PANIC(msg) do { \
    kmsg("PANIC: ", #msg); \
    while(1) {};\
} while(0);

#define WARN(msg) do { \
    kmsg("WARNING: ", #msg); \
} while(0);

#endif // __PANIC_H_
