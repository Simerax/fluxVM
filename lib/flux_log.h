#ifndef FLUX_VM_LOG_H
#define FLUX_VM_LOH_H

#ifndef NDEBUG
#include<stdio.h>
#define FLUX_DLOG(_1, ...) printf("[DEBUG] " _1 "\n", ##__VA_ARGS__)
#define FLUX_ELOG(_1, ...) printf("[ERROR] " _1 "\n", ##__VA_ARGS__)
#define FLUX_WLOG(_1, ...) printf("[WARN]  " _1 "\n", ##__VA_ARGS__)
#else
#define FLUX_DLOG(...)
#define FLUX_ELOG(...)
#define FLUX_WLOG(...)
#endif // NDEBUG

#endif // FLUX_VM_LOG_H
