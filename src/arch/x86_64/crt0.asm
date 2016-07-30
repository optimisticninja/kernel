[SECTION .text]

BITS 64
[GLOBAL _start]
_start:
	mov rbp,0
	push rbp
	push rbp
	mov rbp,rsp

	push rsi
	push rdi
	
	call initialize_standard_library
	
	call _init

	pop rdi
	pop rsi
	
	call main

	mov edi,eax
	call exit
