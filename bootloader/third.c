#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga.h"

void main() {
    term_init();
    term_writestring("loaded third stage...\nsearching for kernel...\n");

    while (1) {
        __asm__("hlt");
    }
}