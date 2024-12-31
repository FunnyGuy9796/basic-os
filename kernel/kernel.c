#include "kernel.h"

bool debug = true;

void main() {
    uint32_t main_addr = (uint32_t)&main;

    term_init();

    kprint("[ ");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_BROWN);
    kprint("KERNEL");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    kprint(" ] loaded basic-os kernel\n");

    kprint("[ ");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_BROWN);
    kprint("KERNEL");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    kprint(" ] initializing system...\n");

    parse_mmap(main_addr);
    pmm_init();

    while (1)
        asm ("hlt");
}