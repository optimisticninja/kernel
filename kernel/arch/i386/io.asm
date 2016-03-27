[global inb]
inb:
	mov edx, [esp+4]
	xor eax, eax
	in al, dx
	ret
	
[global inw]
inw:
	mov edx, [esp+4]
	xor eax, eax
	in ax, dx
	ret
	
[global outb]
outb:
	mov eax, [esp+8]
	mov edx, [esp+4]
	out dx, al
	ret
