#ifndef MMAP_H
#define MMAP_H

#include <stdint.h>
#include "../util/print.h"

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) mmap_entry;

extern uint16_t entry_count;
extern mmap_entry *entries;

void parse_mmap();

#endif