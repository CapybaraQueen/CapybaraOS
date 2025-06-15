#include "tty.h"
#include "vga.h"
#include "stdio.h"
#include "apic.h"
#include "idt.h"

#define __is_libk 1

void kernel_main()
{
	terminal_init();

	if (!check_apic())
	{
		printf("APIC is not supported.\n");
		__asm__ volatile ("cli; hlt");
	}

	idt_init();

	pic_disable();

	enable_apic();

	printf("Welcome to CapybaraOS!\n\nThis is a basic kernel for the OS\n");

	terminal_setcolor(VGA_COLOR_PINK | VGA_COLOR_BLACK << 4);
		
	printf("I love my girlfriend!\n");

	//__asm__ volatile ("int $100");

	printf("BOOP\n");

	//__asm__ volatile ("int $32; hlt");
}
