#ifndef _STDIO_H
#define _STDIO_H 1

#include "cdefs.h"

#define EOF (-1)

#if defined(__cplusplus)
extern "C" {
#endif

int printf(const char* format, ...);
int putchar(int);
int puts(const char*);

#if defined(__cplusplus)
}
#endif

#endif
