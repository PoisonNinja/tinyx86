org 0x1000
start:
	mov sp, 0x2000
	mov ax, msg
	call print
	hlt

print:
        push bx
        push dx
        mov bx, ax

print_loop:
        mov dl, [bx]
        inc bx
        xchg al, dl
        call print_char
        xchg al, dl
        and dl, dl
        jnz print_loop
        pop dx
        pop bx
        ret

print_char:
        push bx
        push di
        mov bx, 8000h
        mov di, [current]
        mov [bx + di], al
        inc di
        mov [current], di
        pop di
        pop bx
        ret

msg:
	db      "Hello, CSO!", 0

current:
	dw	0
