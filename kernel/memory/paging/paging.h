#ifndef PAGING_H
#define PAGING_H

#include "../../util/panic.h"

#define PAGE_DIR_ADDR 0x1000

void *get_physaddr(void *virt_addr);

void map_page(void *phys_addr, void *virt_addr, unsigned int flags);

#endif