[GLOBAL long_mode_start]
[EXTERN kmain]

[SECTION .text]
BITS 64
long_mode_start:
    call kmain
    hlt
