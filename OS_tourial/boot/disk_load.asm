; load dh sectors to ES:BX from drive DL 
disk_load:
    pusha
    push dx ; store dx on stack so later can recall how many 
            ; sectors were request to be read 
            ; even if it is altered in the meantime
    mov ah, 0x2 ; BIOS read sector function ,0x2 = read 
    mov al, dh  ; read DH sectors (num of sectors)
    mov ch, 0x00 ; sel Cylinder 0 (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00 ; sel head 0
    mov cl, 0x02 ; start reading from second sector after the boot sector  ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    int 0x13     ; BIOS Interrupt

    jc disk_error  ; jump if error (carry flag set)

    pop dx       ; restore dx from stack 
    cmp dh, al   ; if al (sector read) != DH (sector expected)
    jne disk_error ; display error message
    popa 
    ret 

disk_error:
    mov bx, DISK_ERROR_MSG
    call print
    call print_nl
    call print_hex
    jmp disk_loop

sectors_error:
    mov bx, SECTORS_ERROR
    call print 

disk_loop:
    jmp $

; variable 
DISK_ERROR_MSG db "Disk read error!", 0
SECTORS_ERROR: db "incorrect number of secotrs read", 0
