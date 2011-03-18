#include <assert.h>

#include <arch/cpu.h>
#include <arch/interrupt.h>
#include <kernel/thread.h>
#include <kernel/thread_queue.h>

thread_t *thread_init(
    thread_t *thread,
    thread_func_t entry_point,
    void *stack,
    uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f
    )
{
    assert(thread);

    threadq_node_init(&thread->queue_node, thread);

    uint64_t *rsp = stack;
    if(rsp != NULL)
    {
        // TODO: push a stub return address to the stack so the thread can return
        *(rsp--) = 0;
    }

    thread->interrupt_stack_frame.rip = (uint64_t)((void*)entry_point);
    thread->interrupt_stack_frame.cs = 0x08; // 64-bit code segment, TODO: don't use a magic constant here
    thread->interrupt_stack_frame.rflags = RFLAGS_NO_FLAGS | RFLAGS_INTERRUPT; // TODO: specify this as a parameter?
    thread->interrupt_stack_frame.rsp = (uint64_t)rsp;
    thread->interrupt_stack_frame.ss = 0x00; // null segment, TODO: don't use a magic constant here

    thread->registers.rdi = a;
    thread->registers.rsi = b;
    thread->registers.rdx = c;
    thread->registers.rcx = d;
    thread->registers.r8 = e;
    thread->registers.r9 = f;

    return thread;
}

// TODO: void thread_finished(thread_t *thread, uint64_t return_value);
