#ifndef MMAP_H
#define MMAP_H

#include <stdint.h>
#include "../../kernel.h"
#include "../../util/print.h"
#include "../../util/panic.h"

extern char end_of_kernel;

typedef struct {
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) mmap_entry_t;

extern uint16_t entry_count;
extern mmap_entry_t *entries;
extern mmap_entry_t *first_usable_entry;
extern uint32_t total_memory;
extern unsigned long kernel_end;

void parse_mmap();

#endif