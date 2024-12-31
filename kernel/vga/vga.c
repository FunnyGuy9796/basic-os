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
    terminal_buffer = (uint16_t*)0x7ffe000;

    term_clear();
}

void term_setcolor(enum vga_color bg, enum vga_color fg) {
    terminal_color = vga_entry_color(fg, bg);
}

void term_move_cursor(size_t x, size_t y) {
    size_t position = y * VGA_WIDTH + x;

    outb(0x3d4, 0x0e);
    outb(0x3d5, (position >> 8) & 0xff);

    outb(0x3d4, 0x0f);
    outb(0x3d5, position & 0xff);
}

void term_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;

    terminal_buffer[index] = vga_entry(c, color);

    term_move_cursor(x + 1, y);
}

void term_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++)
            term_putentryat(' ', terminal_color, x, y);
    }

    terminal_row = 0;
    terminal_column = 0;

    term_move_cursor(0, 0);
}

void term_scroll() {
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t curr_index = y * VGA_WIDTH + x;
            const size_t new_line_index = (y + 1) * VGA_WIDTH + x;

            terminal_buffer[curr_index] = terminal_buffer[new_line_index];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;

        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void term_newline() {
    terminal_column = 0;

    if (terminal_row + 1 == VGA_HEIGHT) {
        term_scroll();

        terminal_row = VGA_HEIGHT - 1;
    } else
        terminal_row++;
    
    term_move_cursor(terminal_column, terminal_row);
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