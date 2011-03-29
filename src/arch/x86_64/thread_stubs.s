    .code64
    .section .text
    .global thread_context_save
# rdi = registers_t*, rsi = interrupt_stack_frame_t*
thread_context_save:
# store general purpose registers to *rdi
    mov %r15, 0(%rdi)
    mov %r14, 8(%rdi)
    mov %r13, 16(%rdi)
    mov %r12, 24(%rdi)
    mov %r11, 32(%rdi)
    mov %r10, 40(%rdi)
    mov %r9, 48(%rdi)
    mov %r8, 56(%rdi)

    mov %rdi, 64(%rdi)
    mov %rsi, 72(%rdi)
    mov %rsp, 80(%rdi)
    mov %rbp, 88(%rdi)
    mov %rdx, 96(%rdi)
    mov %rcx, 104(%rdi)
    mov %rbx, 112(%rdi)
    movq $0, 120(%rdi) # replace %rax with return value of thread_context_switch (false)

# store interrupt stack frame to *rsi: rip, cs, rflags, rsp, ss
    mov (%rsp), %rax # return address of thread_context_save
    mov %rax, 0(%rsi) # offsetof(interrupt_stack_frame_t, rip) = 0
    mov %cs, %rax
    mov %rax, 8(%rsi) # offsetof(interrupt_stack_frame_t, cs) = 8
    pushfq # store rflags
    pop %rax
    mov %rax, 16(%rsi) # offsetof(interrupt_stack_frame_t, rflags) = 16
    mov %rsp, %rax # store stack pointer
    add $8, %rax
    mov %rax, 24(%rsi) # offsetof(interrupt_stack_frame_t, rsp) = 24
    mov %ss, %rax
    mov %rax, 32(%rsi) # offsetof(interrupt_stack_frame_t, ss) = 32

# return true
    mov $1, %rax
    ret

    .global thread_context_restore
# rdi = const registers_t*, rsi = const interrupt_stack_frame*
thread_context_restore:
# restore interrupt stack frame from *rsi and push to stack
# push lret stack frame: rip, cs, rsp, ss
    push 32(%rsi) # offsetof(interrupt_stack_frame_t, ss) = 32
    push 24(%rsi) # offsetof(interrupt_stack_frame_t, rsp) = 24
    push 8(%rsi) # offsetof(interrupt_stack_frame_t, cs) = 8
    push 0(%rsi) # offsetof(interrupt_stack_frame_t, rip) = 0
# push rflags to stack
    push 16(%rsi) # offsetof(interrupt_stack_frame_t, rflags) = 16

# restore general purpose registers from *rdi
    mov 0(%rdi), %r15
    mov 8(%rdi), %r14
    mov 16(%rdi), %r13
    mov 24(%rdi), %r12
    mov 32(%rdi), %r11
    mov 40(%rdi), %r10
    mov 48(%rdi), %r9
    mov 56(%rdi), %r8

    mov 72(%rdi), %rsi
# rsp is omitted
    mov 88(%rdi), %rbp
    mov 96(%rdi), %rdx
    mov 104(%rdi), %rcx
    mov 112(%rdi), %rbx
    mov 120(%rdi), %rax
    mov 64(%rdi), %rdi # rdi is restored last

# pop flags and do an inter privilege level return
    popfq
    lretq $0
