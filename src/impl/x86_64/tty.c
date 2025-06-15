#include<stddef.h>
#include<stdbool.h>
#include<stdint.h>

#include "string.h"
#include "tty.h"
#include "vga.h"

const static size_t VGA_COLS = 80;
const static size_t VGA_ROWS = 25;
static struct Char* const VGA_MEMORY = (struct Char*) 0xb8000;

static size_t ter_col;
static size_t ter_row;
static uint8_t ter_color;
static struct Char volatile* ter_buffer;

struct Char
{
	uint8_t character;
	uint8_t color;
};

void terminal_init(void)
{
	ter_col = 0;
	ter_row = 0;
	ter_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	ter_buffer = VGA_MEMORY;

	terminal_clear();
}

void terminal_setcolor(uint8_t color)
{
	ter_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_COLS + x;
	ter_buffer[index] = (struct Char) 
	{
		.character = (uint8_t) c,
		.color = color,
	};
}

void terminal_newline(void)
{
	ter_col = 0;
	if (ter_row < VGA_ROWS -1)
	{
		ter_row++;
		return;
	}

	for (size_t row = 0; row < VGA_ROWS; row++)
	{
		for (size_t col = 0; col < VGA_COLS; col++)
		{
			struct Char character = ter_buffer[col + VGA_COLS * row];
			ter_buffer[col + VGA_COLS * (row - 1)] = character;
		}
	}

	terminal_clearrow(VGA_COLS - 1);
}

void terminal_putchar(char c)
{
	unsigned char uc = c;

	if (uc == '\n')
	{
		terminal_newline();
		return;
	}

	if (ter_col > VGA_COLS - 1)
	{
		terminal_newline();
	}

	terminal_putentryat(uc, ter_color, ter_col, ter_row);
	if (++ter_col == VGA_COLS)
	{
		terminal_newline();
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_clearrow(size_t row)
{
	struct Char empty = (struct Char)
	{
		.character = ' ',
		.color = ter_color,
	};

	for (size_t col = 0; col < VGA_COLS; col++)
	{
		ter_buffer[col + VGA_COLS * row] = empty;
	}
}

void terminal_clear(void)
{
	for (size_t row = 0; row < VGA_ROWS; row++)
	{
		terminal_clearrow(row);
	}
}

/*struct Char
{
	uint8_t character;
	uint8_t color;
};

struct Char volatile* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clearRow(size_t row)
{
	struct Char empty = (struct Char)
	{
		.character = ' ',
		.color = color,
	};

	for (size_t col = 0; col < NUM_COLS; col++)
	{
		buffer[col + NUM_COLS * row] = empty;
	}
}

void printClear()
{
	for (size_t i = 0; i < NUM_ROWS; i++)
	{
		clearRow(i);
	}
}

void printNewline()
{
	col = 0;
	
	if (row < NUM_ROWS - 1)
	{
		row++;
		return;
	}

	for (size_t row = 0; row < NUM_ROWS - 1; row++)
	{
		for (size_t col = 0; col < NUM_COLS; col++)
		{
			struct Char character = buffer[col + NUM_COLS * row];
			buffer[col + NUM_COLS * (row - 1)] = character;
		}
	}

	clearRow(NUM_COLS -1);
}

void printChar(char character)
{
	if (character == '\n')
	{
		printNewline();
		return;
	}
	
	if (col > NUM_COLS - 1)
	{
		printNewline();
	}

	buffer[col + NUM_COLS * row] = (struct Char)
	{
		.character = (uint8_t) character,
		.color = color,
	};

	col++;
}

void printStr(char* string)
{
	for (size_t i = 0; 1; i++)
	{
		char character = (uint8_t) string[i];

		if (character == '\0')
		{
			return;
		}

		printChar(character);
	}
}

void printSetColor(uint8_t foreground, uint8_t background)
{
	color = foreground + (background << 4);
}*/
