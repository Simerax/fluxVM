#ifndef FLUX_VM_LOG_H
#define FLUX_VM_LOH_H

#ifndef NDEBUG
#include<stdio.h>
#include<string.h>
// https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

#define FLUX_DLOG(_1, ...) printf("[DEBUG] %s:%d " _1 "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define FLUX_ELOG(_1, ...) printf("[ERROR] %s:%d " _1 "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define FLUX_WLOG(_1, ...) printf("[WARN]  %s:%d " _1 "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
#define FLUX_DLOG(...)
#define FLUX_ELOG(...)
#define FLUX_WLOG(...)
#endif // NDEBUG

#endif // FLUX_VM_LOG_H
