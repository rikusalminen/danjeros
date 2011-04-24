#include <arch/interrupt.h>
#include <kernel/thread.h>
#include <kernel/scheduler.h>
#include <kernel/spinlock.h>
#include <kernel/thread_queue.h>

typedef struct scheduler_t
{
    spinlock_t spinlock;
    threadq_t queue;
} scheduler_t;

typedef struct scheduler_cpu_t
{
    thread_t *current_thread;
    thread_t idle_thread;
} scheduler_cpu_t;

static scheduler_t scheduler;
static scheduler_cpu_t scheduler_cpu;
static thread_t main_thread;

static uint8_t idle_thread_stack[1024];
static void idle_task(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;

    while(true)
    {
        // TODO: some nop's here
        scheduler_yield(false);
    }
}

void scheduler_init()
{
    spinlock_init(&scheduler.spinlock);
    threadq_init(&scheduler.queue);

    thread_init(&main_thread, NULL, NULL, 0, 0, 0, 0, 0, 0);
    scheduler_cpu.current_thread = &main_thread;

    thread_init(&scheduler_cpu.idle_thread, idle_task, idle_thread_stack + sizeof(idle_thread_stack),  0, 0, 0, 0, 0, 0);
}

void scheduler_yield(bool ready)
{
    interrupt_state_t interrupt_state = interrupt_disable();

    thread_t* prev_thread = scheduler_cpu.current_thread;
    if(thread_context_save(&prev_thread->registers, &prev_thread->interrupt_stack_frame))
    {
        spinlock_lock(&scheduler.spinlock);

        if(ready)
            threadq_enqueue(&scheduler.queue, &scheduler_cpu.current_thread->queue_node);

        threadq_node_t *next_node = threadq_dequeue(&scheduler.queue);

        spinlock_unlock(&scheduler.spinlock);

        thread_t *next_thread = next_node != NULL ? next_node->thread : &scheduler_cpu.idle_thread;
        scheduler_cpu.current_thread = next_thread;

        if(prev_thread != next_thread)
            thread_context_restore(&next_thread->registers, &next_thread->interrupt_stack_frame);
    }

    interrupt_state_restore(interrupt_state);
}

void scheduler_yield_interrupt(registers_t *registers, interrupt_stack_frame_t *interrupt_stack_frame, bool ready)
{
    thread_t* prev_thread = scheduler_cpu.current_thread;
    prev_thread->registers = *registers;
    prev_thread->interrupt_stack_frame = *interrupt_stack_frame;

    spinlock_lock(&scheduler.spinlock);

    if(ready && prev_thread != &scheduler_cpu.idle_thread)
        threadq_enqueue(&scheduler.queue, &prev_thread->queue_node);
    threadq_node_t *next_node = threadq_dequeue(&scheduler.queue);

    spinlock_unlock(&scheduler.spinlock);

    thread_t *next_thread = next_node != NULL ? next_node->thread : &scheduler_cpu.idle_thread;
    scheduler_cpu.current_thread = next_thread;

    if(prev_thread != next_thread)
    {
        *registers = next_thread->registers;
        *interrupt_stack_frame = next_thread->interrupt_stack_frame;
    }
}

void scheduler_add_ready(thread_t *thread)
{
    assert(thread);

    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&scheduler.spinlock);

    threadq_enqueue(&scheduler.queue, &thread->queue_node);

    spinlock_unlock(&scheduler.spinlock);
    interrupt_state_restore(interrupt_state);
}

void scheduler_add_queue(threadq_t *queue)
{
    assert(queue);

    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&scheduler.spinlock);

    threadq_splice(&scheduler.queue, queue);

    spinlock_unlock(&scheduler.spinlock);
    interrupt_state_restore(interrupt_state);
}
