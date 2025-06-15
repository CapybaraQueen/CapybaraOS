#include "idt.h"
#include "isr.h"
#include<stdbool.h>

extern void* isr_stub_table[];
extern void* int_stub_table[];

static bool vectors[IDT_MAX_DESCRIPTORS];

__attribute__((aligned(0x10)))
static idt_entry_t idt[256]; // Create an array of IDT entries aligned for performance

static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
	idt_entry_t* descriptor = &idt[vector];

	descriptor->isr_low = (uint64_t) isr & 0xffff;
	descriptor->kernel_cs = GDT_OFFSET_KERNEL_CODE; // define this when i go back and look
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = ((uint64_t) isr >> 16) & 0xffff;
	descriptor->isr_high = ((uint64_t) isr >> 32) & 0xffffffff;
	descriptor->reserved = 0;
}

void idt_init()
{
	uint8_t numVectors = 32;

	idtr.base = (uintptr_t) &idt[0];
	idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (uint8_t vector = 0; vector < numVectors; vector++)
	{
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
		vectors[vector] = true;
	}

	idt_set_descriptor(100, isr_stub_table[100], 0x8E);
	vectors[100] = true;

	__asm__ volatile ("lidt %0" : : "m"(idtr)); // load the IDT
	__asm__ volatile ("sti"); //set the interrupt flag
}
