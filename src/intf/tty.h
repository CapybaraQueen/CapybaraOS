#ifndef _TTY_H
#define _TTY_H 1

#include<stddef.h>

void terminal_init(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_clearrow(size_t row);
void terminal_clear(void);

void printClear(void);
void printChar(char character);
void printStr(char* string);

#endif
