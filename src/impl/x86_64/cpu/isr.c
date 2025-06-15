#include "isr.h"
#include "stdio.h"

void exception_handler()
{
	__asm__ volatile ("cli; hlt"); // hands the computer
}

void interrupt_handler()
{
	printf("I interrupted");
	__asm__ volatile ("cli; hlt");
	//extern void interrupt_handler_asm();
}
