MBOOT_PAGE_ALIGN	equ 1 << 0
MBOOT_MEM_INFO		equ 1 << 1
MBOOT_HEADER_MAGIC	equ 0x1BADB002
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]


[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

[SECTION .multiboot]
ALIGN 4
mboot:
	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHECKSUM

	dd mboot
	dd code
	dd bss
	dd end
	dd start


[SECTION .text]
[GLOBAL start]
[EXTERN _init]
[EXTERN kmain]
[EXTERN kernel_early]

start:
	mov esp, stack_top	; Set up stack
	
	call kernel_early
	call _init			; Global constructors

	push ebx			; Push multiboot struct onto stack
	cli	
	call kmain
	
	jmp $


[SECTION .bootstrap_stack, nobits]
ALIGN 4
stack_bottom:
	resb 16384
stack_top:
