#ifndef _APIC_H
#define _APIC_H 1

#include<stdbool.h>
#include<stdint.h>

bool check_apic(void);

void cpu_set_apic_base(uintptr_t apic);

void enable_apic(void);

uint32_t cpuReadIOApic(void* ioapicAddr, uint32_t reg);

void cpuWriteIOApic(void* ioapicAddr, uint32_t reg, uint32_t value);

void pic_disable(void);

#endif
