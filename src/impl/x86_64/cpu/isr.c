#include "isr.h"

void exception_handler()
{
	__asm__ volatile ("cli; hlt"); // hands the computer
}

void interrupt_handler()
{
	extern void interrupt_handler_asm();
}
