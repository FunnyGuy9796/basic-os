#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga/vga.h"
#include "util/print.h"
#include "util/panic.h"
#include "memory/mmap/mmap.h"
#include "memory/pmm/pmm.h"

extern bool debug;

#endif