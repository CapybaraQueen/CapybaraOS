#include "tty.h"
#include "vga.h"
#include "stdio.h"

#define __is_libk 1

void kernel_main()
{
	terminal_init();
	printf("Welcome to CapybaraOS!\n\nThis is a basic kernel for the OS\n");

	terminal_setcolor(VGA_COLOR_PINK | VGA_COLOR_BLACK << 4);
		
	printf("I love my girlfriend!\n");
}
