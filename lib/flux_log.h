#ifndef FLUX_VM_LOG_H
#define FLUX_VM_LOH_H

#ifndef NDEBUG
#include<stdio.h>
#define FLUX_DLOG(_1, ...) printf("[DEBUG] " _1 "\n", ##__VA_ARGS__)
#else
#define FLUX_DLOG(...)
#endif // NDEBUG

#endif // FLUX_VM_LOG_H
