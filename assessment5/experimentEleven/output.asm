.model small
.stack 100h
.data
.code
main proc
	mov ax, @data
	mov ds, ax
	mov bp, sp
	sub sp, 200h
	mov ax, [bp-0]
	push ax
	mov ax, [bp-2]
	mov bx, ax
	pop ax
	mov [bp-4], ax
	mov ax, [bp-6]
	push ax
	mov ax, +
	mov bx, ax
	pop ax
	mov [bp-8], ax
	mov ax, [bp-8]
	push ax
	mov ax, -
	mov bx, ax
	pop ax
	mov [bp-10], ax
	mov ax, [bp-10]
	mov [bp-12], ax
	mov ax, 4c00h
	int 21h
main endp
end main
