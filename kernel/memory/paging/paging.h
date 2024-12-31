#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stdbool.h>
#include "../../util/print.h"
#include "../../util/panic.h"
#include "../pmm/pmm.h"

#define PAGE_DIR_ADDR 0x1000

void *get_physaddr(void *virt_addr);

void map_page(void *phys_addr, void *virt_addr, uint32_t flags);

#endif