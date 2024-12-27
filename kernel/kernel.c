#include "kernel.h"

bool debug = true;

void main() {
    term_init();
    printf("loaded basic-os kernel\ninitializing system...\n\n");

    parse_mmap();
    pmm_init();

    while (1)
        asm ("hlt");
}