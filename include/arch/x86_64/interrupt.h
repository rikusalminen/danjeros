#ifndef X86_64_INTERRUPT_H
#define X86_64_INTERRUPT_H

#include <types.h>
#include <arch/interrupt.h>

#define INT_DIVIDE_ERROR    ((interrupt_vector_t)0x0)
#define INT_DEBUG           ((interrupt_vector_t)0x1)
#define INT_NMI             ((interrupt_vector_t)0x2)
#define INT_BREAKPOINT      ((interrupt_vector_t)0x3)
#define INT_OVERFLOW        ((interrupt_vector_t)0x4)
#define INT_BOUNDS_CHECK    ((interrupt_vector_t)0x5)
#define INT_INVALID_OPCODE  ((interrupt_vector_t)0x6)
#define INT_COPROCESSOR_NOT_AVAILABLE   ((interrupt_vector_t)0x7)
#define INT_DOUBLE_FAULT    ((interrupt_vector_t)0x8)
#define INT_COPROSESSOR_SEGMENT_OVERRUN ((interrupt_vector_t)0x9)
#define INT_INVALID_TSS     ((interrupt_vector_t)0xa)
#define INT_SEGMENT_NOT_PRESENT ((interrupt_vector_t)0xb)
#define INT_STACK_EXCEPTION ((interrupt_vector_t)0xc)
#define INT_GENERAL_PROTECTION_EXCEPTION ((interrupt_vector_t)0xd)
#define INT_PAGE_FAULT      ((interrupt_vector_t)0xe)
// Interrupt vector 0xf is Intel reserved
#define INT_COPROSSOR_ERROR ((interrupt_vector_t)0x10)

#define INT_IRQ0            ((interrupt_vector_t)0x20)
#define INT_IRQ1            (INT_IRQ0 + 1)
#define INT_IRQ2            (INT_IRQ0 + 2)
#define INT_IRQ3            (INT_IRQ0 + 3)
#define INT_IRQ4            (INT_IRQ0 + 4)
#define INT_IRQ5            (INT_IRQ0 + 5)
#define INT_IRQ6            (INT_IRQ0 + 6)
#define INT_IRQ7            (INT_IRQ0 + 7)
#define INT_IRQ8            ((interrupt_vector_t)0x28)
#define INT_IRQ9            (INT_IRQ8 + 1)
#define INT_IRQ10           (INT_IRQ8 + 2)
#define INT_IRQ11           (INT_IRQ8 + 3)
#define INT_IRQ12           (INT_IRQ8 + 4)
#define INT_IRQ13           (INT_IRQ8 + 5)
#define INT_IRQ14           (INT_IRQ8 + 6)
#define INT_IRQ15           (INT_IRQ8 + 7)

typedef struct interrupt_stack_frame_t
{
    uint64_t rip, cs, rflags, rsp, ss;
} interrupt_stack_frame_t;

static inline void interrupt_flag_set()
{
    __asm__ __volatile__ ("sti");
}

static inline void interrupt_flag_clear()
{
    __asm__ __volatile__ ("cli");
}

static inline interrupt_state_t interrupt_state_get()
{
    return (rflags_read() & RFLAGS_INTERRUPT) != 0;
}

static inline void interrupt_state_restore(interrupt_state_t state)
{
    if(state) interrupt_flag_set();
    else interrupt_flag_clear();
}

static inline interrupt_state_t interrupt_enable()
{
    interrupt_state_t state = interrupt_state_get();
    interrupt_flag_set();
    return state;
}

static inline interrupt_state_t interrupt_disable()
{
    interrupt_state_t state = interrupt_state_get();
    interrupt_flag_clear();
    return state;
}


void interrupt_init();

#endif
