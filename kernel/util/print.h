#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include "util.h"
#include "../vga/vga.h"

void kprint(const char* format, ...);

#endif