[GLOBAL long_mode_start]
[EXTERN kmain]

BITS 64

[SECTION .text]
long_mode_start:
    call kmain
    hlt
