#include "isr.h"

void exception_handler()
{
	__asm__ volatile ("cli; hlt"); // hands the computer
}
