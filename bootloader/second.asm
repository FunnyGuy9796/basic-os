[bits 16]
[org 0x7e00]

start:
    cli
    mov ax, 0x10
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov ah, 0x00
    mov al, 0x03
    
    mov si, second_msg
    call print

    call detect_memory

    mov si, memory_map_msg
    call print

    call enable_a20
    call check_a20

    mov si, a20_enabled_msg
    call print

    push 0x1000
    pop es
    mov bx, 0x0
    call load_kernel

    call enable_protected_mode

    hlt
    jmp $

enable_a20:
    in al, 0x92
    or al, 0x02
    out 0x92, al

    mov cx, 0x10000
delay:
    loop delay

    ret

check_a20:
    mov ax, 0x1234
    mov [0x100000], ax
    mov ax, [0x100000]
    
    cmp ax, 0x1234
    jne a20_disabled

    ret

a20_disabled:
    mov si, a20_disabled_msg
    call print

    hlt
    jmp $

mmap_entry_count equ 0x9000
mmap_entry_base equ 0x9004
mmap_entry_size equ 24

detect_memory:
    xor di, di
    mov di, mmap_entry_base
    xor ebx, ebx
    xor bp, bp

    xor ax, ax
    mov es, ax
.memory_loop:
    mov edx, 0x534D4150
    mov eax, 0xe820
    mov ecx, mmap_entry_size
    mov [es:di + 20], dword 1

    int 0x15
    jc .memory_error

    cmp eax, edx
    jne .memory_error

    mov ecx, [es:di + 8]
    or ecx, [es:di + 12]
    jz .skip_entry

    add di, mmap_entry_size
    inc bp
.skip_entry:
    test ebx, ebx
    jnz .memory_loop

    mov [es:mmap_entry_count], bp

    clc
    ret
.memory_error:
    mov si, memory_error_msg
    call print

    stc

    hlt
    jmp $

enable_protected_mode:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode_entry

print:
.next_char:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp .next_char
.done:
    ret

load_kernel:
    mov ah, 0x02
    mov al, 17
    mov ch, 0
    mov cl, 4
    mov dh, 0
    mov dl, 0x80

    int 0x13

    jc disk_error

    ret

disk_error:
    mov si, disk_error_msg
    call print

    hlt
    jmp $

gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[bits 32]

%define PAGE_DIR_ADDR 0x1000
%define PAGE_TABLE_ADDR 0x2000

setup_paging:
    mov edi, PAGE_TABLE_ADDR
    xor ebx, ebx
.map_pages:
    mov eax, ebx
    or eax, 0x3
    mov [edi], eax
    add edi, 4
    add ebx, 0x1000
    cmp ebx, 0x400000
    jl .map_pages

    mov eax, 0x9000 | 0x3
    mov [PAGE_TABLE_ADDR + ((0x9000 >> 12) * 4)], eax

    mov eax, 0x10000 | 0x3
    mov [PAGE_TABLE_ADDR + ((0x10000 >> 12) * 4)], eax

    ret

init_page_directory:
    mov edi, PAGE_DIR_ADDR

    xor eax, eax
    mov ecx, 1024
.clear_directory:
    mov [edi], eax
    add edi, 4
    loop .clear_directory

    mov eax, PAGE_TABLE_ADDR | 0x3
    mov [PAGE_DIR_ADDR], eax

    ret

enable_paging:
    mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax
    
    mov eax, PAGE_DIR_ADDR
    mov cr3, eax

    mov eax, cr3
    cmp eax, PAGE_DIR_ADDR
    je .cr3_ok

    mov esi, cr3_error_msg
    call print_pm

    hlt
    jmp $
.cr3_ok:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax

    ret

protected_mode_entry:
    mov ax, 0x10
    mov cs, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x7ffff

    mov edx, 0xb8000

    mov esi, protected_enabled_msg
    call print_pm

    call setup_paging
    call init_page_directory
    call enable_paging

    mov esi, paging_enabled_msg
    call print_pm

    mov ax, cs
    cmp ax, 0x10
    jne segment_error

    jmp 0x08:0x10000

segment_error:
    mov esi, segment_error_msg
    call print_pm
    
    hlt
    jmp $

print_pm:
    mov ebx, edx
    mov ah, 0x0f
.next_char_pm:
    lodsb
    cmp al, 0
    je .done_pm

    mov [ebx], ax
    add ebx, 2
    jmp .next_char_pm
.done_pm:
    mov edx, ebx

    ret

second_msg db 'loaded second stage ', 0

a20_enabled_msg db 'a20 line enabled ', 0
a20_disabled_msg db 'failed to enable a20 line ', 0
memory_map_msg db 'memory map found ', 0
memory_error_msg db 'failed detecting memory ', 0
disk_error_msg db 'failed reading disk ', 0

protected_enabled_msg db 'protected mode enabled ', 0
paging_enabled_msg db 'paging enabled ', 0
cr3_error_msg db 'CR3 register not set successfully ', 0
protected_memory_error_msg db 'protected memory error ', 0
segment_error_msg db 'segment error ', 0