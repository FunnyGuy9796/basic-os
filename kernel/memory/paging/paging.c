#include "paging.h"

void *get_physaddr(void *virt_addr) {
    unsigned long pdindex = (unsigned long)virt_addr >> 22;
    unsigned long ptindex = ((unsigned long)virt_addr >> 12) & 0x03ff;
    unsigned long *pd = (unsigned long*)PAGE_DIR_ADDR;
    unsigned long pde = pd[pdindex];

    if ((pde & 0x1) == 0)
        kpanic(0);
    
    unsigned long *pt = (unsigned long*)((pde & ~0xfff) + (0x400 * pdindex));
    unsigned long pte = pt[ptindex];

    if ((pte & 0x1) == 0)
        kpanic(0);
    
    return (void*)((pte & ~0xfff) + ((unsigned long)virt_addr & 0xfff));
}

void map_page(void *phys_addr, void *virt_addr, unsigned int flags) {
    if ((unsigned long)phys_addr & 0xfff || (unsigned long)virt_addr & 0xfff)
        kpanic(0);
    
    unsigned long pdindex = (unsigned long)virt_addr >> 22;
    unsigned long ptindex = ((unsigned long)virt_addr >> 12) & 0x03ff;
    unsigned long *pd = (unsigned long*)PAGE_DIR_ADDR;
    unsigned long pde = pd[pdindex];
    unsigned long *pt;
    
    
}