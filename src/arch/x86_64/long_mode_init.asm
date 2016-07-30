[GLOBAL long_mode_start]
[EXTERN main]

[SECTION .text]
BITS 64
long_mode_start:
    call main
    hlt
