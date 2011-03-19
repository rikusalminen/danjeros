#ifndef X86_64_CPU_H
#define X86_64_CPU_H

#include <stdint.h>

#include <arch/cpu.h>

typedef uint16_t segment_selector_t;

typedef enum cpu_privilege_t
{
    PRIVILEGE_RING0 = 0,
    PRIVILEGE_RING1 = 1,
    PRIVILEGE_RING2 = 2,
    PRIVILEGE_RING3 = 3,
    PRIVILEGE_HIGHEST = PRIVILEGE_RING0,
    PRIVILEGE_LOWEST = PRIVILEGE_RING3
} cpu_privilege_t;

typedef struct registers_t
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8,
             rdi, rsi, rsp, rbp, rdx, rcx, rbx, rax;
} registers_t;

typedef uint64_t rflags_t;

#define RFLAGS_CARRY        ((rflags_t) 1 << 0)
#define RFLAGS_NO_FLAGS     ((rflags_t) 1 << 1) // must always be set
#define RFLAGS_PARITY       ((rflags_t) 1 << 2)
#define RFLAGS_ADJUST       ((rflags_t) 1 << 4)
#define RFLAGS_ZERO         ((rflags_t) 1 << 6)
#define RFLAGS_SIGN         ((rflags_t) 1 << 7)
#define RFLAGS_TRAP         ((rflags_t) 1 << 8)
#define RFLAGS_INTERRUPT    ((rflags_t) 1 << 9)
#define RFLAGS_DIRECTION    ((rflags_t) 1 << 10)
#define RFLAGS_OVERFLOW     ((rflags_t) 1 << 11)

#define RFLAGS_IOPL         ((rflags_t) 3 << 12)
#define RFLAGS_NESTED_TASK  ((rflags_t) 1 << 14)

#define RFLAGS_RESUME       ((rflags_t) 1 << 16)
#define RFLAGS_VIRTUAL_X86  ((rflags_t) 1 << 17)
#define RFLAGS_ALIGNMENT_CHECK  ((rflags_t) 1 << 18)
#define RFLAGS_VIRTUAL_INTERRUPT    ((rflags_t) 1 << 19)
#define RFLAGS_VIRTUAL_INTERRUPT_PENDING  ((rflags_t) 1 << 20)
#define RFLAGS_IDENTIFICATION   ((rflags_t) 1 << 21)

static inline rflags_t rflags_read()
{
    rflags_t rflags;
    __asm__ __volatile__ ("pushfq ; pop %0" : "=g" (rflags));
    return rflags;
}

static inline void cpu_pause()
{
    __asm__ __volatile__ ("pause");
}

#endif
