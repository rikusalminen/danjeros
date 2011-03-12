    .intel_syntax noprefix

    .section .text
    .code32

    .global start

start:
    cli

    # print a '6' to the upper left corner and halt
    mov eax, 0xb8000
    mov bx, (0x0700 | 0x36)
    mov [eax], bx

hang:
    hlt
    jmp hang


