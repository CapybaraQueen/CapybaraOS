#ifndef _ISR_H
#define _ISR_H 1

__attribute__((noreturn))
void exception_handler(void);

__attribute__((noreturn))
void interrupt_handler(void);

#endif
