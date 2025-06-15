#include "apic.h"
#include<cpuid.h>
#include<stdint.h>
#include<stdbool.h>
#include "stdio.h"

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

extern int check_apic_asm();
extern void set_apic_base_asm(uintptr_t apic);
extern uintptr_t get_apic_base_asm();
extern void enable_apic_asm();

bool check_apic()
{
	//uint32_t eax, edx;
	//cpuid(1, &eax, &edx);
	//return edx & CPUID_FEAT_EDX_APIC;
	return (bool) check_apic_asm();
}

void cpu_set_apic_base(uintptr_t apic)
{
	uint32_t edx = (apic >> 32) & 0x0f;
	uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

	wrmsr_edx_eax(IA32_APIC_BASE_MSR, edx, eax);
	//set_apic_base_asm(apic);
}

uintptr_t cpu_get_apic_base()
{
	uint32_t eax, edx;
	rdmsr_edx_eax(IA32_APIC_BASE_MSR, &edx, &eax);

	return (uintptr_t) (eax & 0xfffff000) | ((edx & 0x0f) << 32);
	//return (uintptr_t) get_apic_base_asm();
}

void enable_apic()
{
	cpu_set_apic_base(cpu_get_apic_base());

	*(volatile uint32_t*) 0xf0 = *(volatile uint32_t*) 0xf0 | (volatile uint32_t) 0x100;
	//enable_apic_asm();
}

uint32_t cpuTeadIOApic(void* ioapicAddr, uint32_t reg)
{
	uint32_t volatile* ioapic = (uint32_t volatile*) ioapicAddr;
	ioapic[0] = (reg & 0xff);
	return ioapic[4];
}

void cpuWriteIOApic(void *ioapicAddr, uint32_t reg, uint32_t value)
{
	uint32_t volatile* ioapic = (uint32_t volatile*) ioapicAddr;
	ioapic[0] = (reg & 0xff);
	ioapic[4] = value;
}

void pic_disable()
{
	outb(0x21, 0xff);
	outb(0xA1, 0xff);
	//__asm__ volatile ("movw $0x21, $0xff");
	//__asm__ volatile ("movw $0xA0, $0xff");
}
