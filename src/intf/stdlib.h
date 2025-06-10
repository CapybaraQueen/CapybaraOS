#ifndef _STDLIB_H
#define _STDLIB_H 1

#include<stddef.h>
#include "cdefs.h"

#if defined(__cplusplus)
extern "C" {
#endif

// atoi(), abort(), abs(), div(), malloc(), free(), calloc(), exit(), realloc(), srand(), rand(), qsort()

int atoi(const char* str);

void* malloc(size_t size);

void free(void* ptr);

void abort(void);

void exit(int status);

#if defined(__cplusplus)
}
#endif

#endif
