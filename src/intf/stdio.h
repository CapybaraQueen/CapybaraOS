#ifndef _STDIO_H
#define _STDIO_H 1

#include "cdefs.h"
#include<stdint.h>

#define EOF (-1)
#define fence() __asm__ volatile ("":::"memory")

#if defined(__cplusplus)
extern "C" {
#endif

int printf(const char* format, ...);
int putchar(int);
int puts(const char*);

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ("inb %w1, %b0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}

static inline void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

static inline void wrmsr(uint64_t msr, uint64_t value)
{
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
    );
}

static inline void wrmsr_edx_eax(uint64_t msr, uint32_t high, uint32_t low)
{
   asm volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
    );
}


static inline uint64_t rdmsr(uint64_t msr)
{
    uint32_t low, high;
    asm volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr)
    );
	return ((uint64_t)high << 32) | low;
}

static inline void rdmsr_edx_eax(uint64_t msr, uint32_t* high, uint32_t* low)
{
    asm volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr)
    );
}

#if defined(__cplusplus)
}
#endif

#endif
