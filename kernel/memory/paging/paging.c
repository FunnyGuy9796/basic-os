#include "paging.h"

bool pt_empty(uint32_t *page_table) {
    for (int i = 0; i < 1024; i++) {
        if (page_table[i] != 0)
            return false;
    }

    return true;
}

bool is_aligned(void *addr) {
    return ((uint32_t)addr & 0xfff) == 0;
}

void *get_physaddr(void *virt_addr) {
    uint32_t pdindex = (uint32_t)virt_addr >> 22;
    uint32_t ptindex = ((uint32_t)virt_addr >> 12) & 0x03ff;
    uint32_t *pd = (uint32_t*)PAGE_DIR_ADDR;
    uint32_t pde = pd[pdindex];

    if ((pde & 0x1) == 0)
        kpanic(0);
    
    uint32_t *pt = (uint32_t*)((pde & ~0xfff) + (0x400 * pdindex));
    uint32_t pte = pt[ptindex];

    if ((pte & 0x1) == 0)
        kpanic(0);
    
    return (void*)((pte & ~0xfff) + ((uint32_t)virt_addr & 0xfff));
}

void map_page(void *phys_addr, void *virt_addr, uint32_t flags) {
    if (!is_aligned(phys_addr) || !is_aligned(virt_addr))
        kpanic(0);
    
    uint32_t pdindex = (uint32_t)virt_addr >> 22;
    uint32_t ptindex = ((uint32_t)virt_addr >> 12) & 0x03ff;
    uint32_t *pd = (uint32_t*)PAGE_DIR_ADDR;
    uint32_t pde = pd[pdindex];

    if ((pde & 0x1) == 0) {
        uint32_t new_pt_phys = pmm_alloc_frame();

        if (new_pt_phys == 0)
            kpanic(0);

        uint32_t *new_pt = (uint32_t*)(new_pt_phys);

        memset(new_pt, 0, 1024 * sizeof(uint32_t));

        pd[pdindex] = new_pt_phys | (flags & 0xfff) | 0x1;
        pde = pd[pdindex];  
    }

    uint32_t *pt = (uint32_t*)((pde & ~0xfff));

    if (pt[ptindex] & 0x1) {
        kprint("[ ");
        term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
        kprint("WARNING");
        term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
        kprint(" ] page already mapped at 0x%x\n", (uint32_t)virt_addr);

        return;
    }
    
    pt[ptindex] = ((uint32_t)phys_addr & ~0xfff) | (flags & 0xfff) | 0x1;

    asm volatile ("invlpg (%0)" :: "r"(virt_addr) : "memory");
}

void unmap_page(void *virt_addr) {
    if (!is_aligned(virt_addr))
        kpanic(0);

    uint32_t phys_addr = (uint32_t)get_physaddr(virt_addr);

    if (phys_addr != 0)
        pmm_free_frame(phys_addr);
    
    uint32_t pdindex = (uint32_t)virt_addr >> 22;
    uint32_t ptindex = ((uint32_t)virt_addr >> 12) & 0x03ff;
    uint32_t *pd = (uint32_t*)PAGE_DIR_ADDR;
    uint32_t pde = pd[pdindex];

    if ((pde & 0x1) == 0)
        return;
    
    uint32_t *pt = (uint32_t*)((pde & ~0xfff) + (0x400 * pdindex));
    uint32_t pte = pt[ptindex];

    if ((pte & 0x1) == 0)
        return;
    
    pt[ptindex] = 0;

    if (pt_empty(pt)) {
        pmm_free_frame((uint32_t)(pde & ~0xfff));
        pd[pdindex] = 0;
    }

    asm volatile ("invlpg (%0)" :: "r"(virt_addr) : "memory");
}
