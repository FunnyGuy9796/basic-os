#include "kernel.h"

void main() {
    term_init();
    printf("loaded kernel v%d\ninitializing system...\n", 1);

    parse_mmap();

    while (1) {
        __asm__("hlt");
    }
}