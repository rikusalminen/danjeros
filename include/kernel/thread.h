#ifndef THREAD_H
#define THREAD_H

#include <arch/cpu.h>
#include <arch/interrupt.h>
#include <kernel/thread_queue.h>

typedef struct thread_t
{
    registers_t registers;
    interrupt_stack_frame_t interrupt_stack_frame;
    threadq_node_t queue_node;
} thread_t;

typedef void (*thread_func_t)(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f);

thread_t *thread_init(
    thread_t *thread,
    thread_func_t entry_point,
    void *stack,
    uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f
    );

#endif
