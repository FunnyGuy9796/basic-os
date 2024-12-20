#include "vga.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

size_t strlen(const char* str) {
	size_t len = 0;

	while (str[len])
		len++;
	
	return len;
}

void term_init() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)0xb8000;

    term_clear();
}

void term_setcolor(uint8_t color) {
    terminal_color = color;
}

void term_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;

    terminal_buffer[index] = vga_entry(c, color);
}

void term_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            term_putentryat(' ', terminal_color, x, y);
        }
    }
}

void term_newline() {
    terminal_column = 0;

    if (++terminal_row == VGA_HEIGHT)
        terminal_row = 0;
}

void term_putchar(char c) {
    if (c == '\n')
        term_newline();
    else {
        term_putentryat(c, terminal_color, terminal_column, terminal_row);

        if (++terminal_column == VGA_WIDTH)
            term_newline();
    }
}

void term_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        term_putchar(data[i]);
}

void term_writestring(const char* data) {
    term_write(data, strlen(data));
}