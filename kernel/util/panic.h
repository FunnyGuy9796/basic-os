#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>
#include "../vga/vga.h"
#include "print.h"

void kpanic(unsigned int eip);

#endif