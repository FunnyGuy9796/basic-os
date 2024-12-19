[bits 16]
[org 0x7e00]

start:
    mov ah, 0x00
    mov al, 0x03
    
    mov si, second_msg
    call print

    call enable_a20
    call check_a20

    mov si, a20_enabled_msg
    call print

    call enable_protected_mode

    hlt

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
    mov [0x1000], ax
    mov ax, [0x1000]
    cmp ax, 0x1234
    jne a20_disabled

    ret

a20_disabled:
    mov si, a20_disabled_msg
    call print

    hlt

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
    cmp ebx, 0x100000
    jl .map_pages

    ret

clear_page_table:
    mov edi, PAGE_TABLE_ADDR
    xor eax, eax
    mov ecx, 1024
.clear_page_table_loop:
    mov [edi], eax
    add edi, 4
    loop .clear_page_table_loop
    
    ret

init_page_directory:
    mov edi, PAGE_DIR_ADDR

    xor eax, eax
    mov ecx, 1024
.clear_directory:
    mov [edi], eax
    add edi, 4
    loop .clear_directory

    mov eax, PAGE_TABLE_ADDR
    or eax, 0x3
    mov [PAGE_DIR_ADDR], eax

    ret

enable_paging:
    mov eax, PAGE_DIR_ADDR
    mov cr3, eax

    mov eax, cr3
    cmp eax, PAGE_DIR_ADDR
    je .cr3_ok

    mov esi, cr3_error_msg
    call print_pm

    hlt
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
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x9000

    mov edx, 0xb8000

    mov esi, protected_enabled_msg
    call print_pm

    call clear_page_table
    call setup_paging
    call init_page_directory
    call enable_paging

    mov esi, paging_enabled_msg
    call print_pm

    hlt

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

second_msg db 'loaded second stage... ', 0

a20_enabled_msg db 'a20 line enabled... ', 0
a20_disabled_msg db 'failed to enable a20 line... ', 0

protected_enabled_msg db 'protected mode enabled... ', 0
paging_enabled_msg db 'paging enabled... ', 0
cr3_error_msg db 'CR3 register not set successfully... ', 0