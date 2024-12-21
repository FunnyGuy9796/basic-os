[bits 16]
[org 0x7c00]

.start:
    mov si, boot_msg
    call print

    call load_second

    jmp $

print:
next_char:
    lodsb
    or al, al
    jz done
    mov ah, 0x0e
    mov bx, 0x0007
    int 0x10
    jmp next_char
done:
    ret

load_second:
    mov ah, 0x02
    mov al, 2
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x7e00

    int 0x13

    jc disk_error

    jmp 0x7e00

disk_error:
    mov si, error_msg
    call print

    jmp $

boot_msg db 'loading second stage... ', 0
error_msg db 'disk read error ', 0

times 510-($-$$) db 0
dw 0xaa55