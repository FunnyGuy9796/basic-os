#ifndef MMAP_H
#define MMAP_H

#include <stdint.h>
#include "../util/print.h"

typedef struct {
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) mmap_entry;

extern uint16_t entry_count;
extern mmap_entry *entries;

void parse_mmap();

#endif