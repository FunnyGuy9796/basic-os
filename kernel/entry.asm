bits 32

extern kmain

section .text.startup

entry:

    ; Enable the x87 FPU (assumes one is present)
    mov eax, cr4
    or eax, 0x200
    mov cr4, eax
    fldcw [fpu_cw]

    cld                        ; Required by the i386 System V ABI
    call kmain                 ; Call kernel entry point

; Infinite loop
.hltloop:
    hlt
    jmp .hltloop

fpu_cw:
    dw 0x37f

