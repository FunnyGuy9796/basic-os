#include "panic.h"

unsigned int eax, ebx, ecx, edx, esi, edi, ebp, esp, eip, eflags;
unsigned int cr0, cr2, cr3, cr4;
unsigned short cs, ds, ss;

__attribute__((optimize("O0"))) void save_regs() {
    asm volatile ("mov %%eax, %0" : "=r"(eax));
    asm volatile ("mov %%ebx, %0" : "=r"(ebx));
    asm volatile ("mov %%ecx, %0" : "=r"(ecx));
    asm volatile ("mov %%edx, %0" : "=r"(edx));
    asm volatile ("mov %%esi, %0" : "=r"(esi));
    asm volatile ("mov %%edi, %0" : "=r"(edi));
    asm volatile ("mov %%ebp, %0" : "=r"(ebp));
    asm volatile ("mov %%esp, %0" : "=r"(esp));
    asm volatile ("pushf; pop %0" : "=r"(eflags));
    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("mov %%ds, %0" : "=r"(ds));
    asm volatile ("mov %%ss, %0" : "=r"(ss));

    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    asm volatile ("mov %%cr2, %0" : "=r"(cr2));
    asm volatile ("mov %%cr3, %0" : "=r"(cr3));
    asm volatile ("mov %%cr4, %0" : "=r"(cr4));
}

__attribute__((optimize("O0"))) void kpanic(unsigned int eip) {
    save_regs();

    term_setcolor(VGA_COLOR_BLUE, VGA_COLOR_WHITE);
    term_clear();

    kprint("\n\n--------------------------------- KERNEL PANIC ---------------------------------\n\nthe system has encountered a critical error and cannot continue\nplease reboot manually\n");
    kprint("\n\n---------------------------------- PANIC INFO ----------------------------------\n\n");

    kprint("EAX: %x  EBX: %x  ECX: %x  EDX: %x  ESI: %x  EDI: %x  EBP: %x  ESP: %x  EFLAGS: %x  CS: %x  DS: %x  SS: %x  CR0: %x  CR2: %x  CR3: %x  CR4: %x\n\n", 
    eax, ebx, ecx, edx, esi, edi, ebp, esp, eflags, cs, ds, ss, cr0, cr2, cr3, cr4);

    if (eip == 0)
        kprint("EIP: unable to retrieve EIP value (non-interrupt panic)\n");
    else
        kprint("EIP: %x\n", eip);

    while (1)
        asm ("hlt");
}
