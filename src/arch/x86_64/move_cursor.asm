BITS 64
BITS 64

[GLOBAL move_cursor]
move_cursor:     
	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	
	;unsigned short position = (row*80) + col;
	;AX will contain 'position'
	mov ax,bx
	and ax,0ffh             ;set AX to 'row'
	mov cl,80   
	mul cl                  ;row*80
	
	mov cx,bx               
	shr cx,8                ;set CX to 'col'
	add ax,cx               ;+ col
	mov cx,ax               ;store 'position' in CX
	
	;cursor LOW port to vga INDEX register
	mov al,0fh             
	mov dx,3d4h             ;VGA port 3D4h
	out dx,al             
	
	mov ax,cx               ;restore 'postion' back to AX  
	mov dx,3d5h             ;VGA port 3D5h
	out dx,al               ;send to VGA hardware
	
	;cursor HIGH port to vga INDEX register
	mov al,0eh
	mov dx,3d4h             ;VGA port 3D4h
	out dx,al
	
	mov ax,cx               ;restore 'position' back to AX
	shr ax,8                ;get high byte in 'position'
	mov dx,3d5h             ;VGA port 3D5h
	out dx,al               ;send to VGA hardware
	
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
	ret
