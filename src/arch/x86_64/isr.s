    .extern interrupt_handler

    .section .text
    .code64

isr_stub:
# save general purpose registers
    push %rax
    mov %rsp, %rax # store old stack ptr at rax
    add $8, %rax
    push %rbx
    push %rcx
    push %rdx
    push %rbp
    push %rax # rsp
    push %rsi
    push %rdi

    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

# set interrupt handler parameters and call interrupt handler
    mov 0(%rax), %rdi   # interrupt vector number
    mov 8(%rax), %rsi   # interrupt error code
    mov %rsp, %rdx      # general purpose registers
    mov %rax, %rcx      # interrupt stack frame
    add $16, %rcx
    call interrupt_handler

# restore general purpose registers
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8

    pop %rdi
    pop %rsi
    add $8, %rsp # rsp
    pop %rbp
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

# pop interrupt vector number and error code
    add $16, %rsp

    iretq


    .global INTERRUPT_MAX
    .set INTERRUPT_MAX, 64
    .global INTERRUPT_ISR_ALIGN
    .set INTERRUPT_ISR_ALIGN, 16

    .macro generate_isr n
    .if \n < INTERRUPT_MAX
        .align INTERRUPT_ISR_ALIGN
    isr\n:
    .if \n <= 7 || \n == 9 || \n == 16 || \n >= 18
        push $0
    .endif
        push $\n
        jmp isr_stub
    .endif
    .endm

    .global isr0x00

# call generate_isr macro with n = 0x00 .. 0xff
    .irpc x, 0123456789abcdef
    .irpc y, 0123456789abcdef
    generate_isr 0x\x\y
    .endr
    .endr


    .section .bss

    .set INTERRUPT_DESCRIPTOR_SIZE, 16
    .global interrupt_descriptor_table
    .align 16
interrupt_descriptor_table:
    .space INTERRUPT_MAX * INTERRUPT_DESCRIPTOR_SIZE

