#ifndef MUTEX_H
#define MUTEX_H

#include <stdbool.h>

#include <kernel/spinlock.h>
#include <kernel/thread_queue.h>
#include <kernel/scheduler.h>

typedef struct mutex_t
{
    spinlock_t spinlock;
    bool locked;
    threadq_t queue;
} mutex_t;

#define MUTEX_INIT { .spinlock = SPINLOCK_INIT, .locked = false, .queue = THREADQ_INIT }

static inline mutex_t* mutex_init(mutex_t *mutex)
{
    spinlock_init(&mutex->spinlock);
    threadq_init(&mutex->queue);
    mutex->locked = false;

    return mutex;
}

static inline void mutex_lock(mutex_t *mutex)
{
    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&mutex->spinlock);

    while(mutex->locked)
    {
        spinlock_unlock(&mutex->spinlock);

        thread_t *this_thread = scheduler_current_thread();
        threadq_enqueue(&mutex->queue, &this_thread->queue_node);
        scheduler_yield(false);

        spinlock_lock(&mutex->spinlock);
    }

    mutex->locked = true;

    spinlock_unlock(&mutex->spinlock);
    interrupt_state_restore(interrupt_state);
}

static inline void mutex_unlock(mutex_t *mutex)
{
    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&mutex->spinlock);

    assert(mutex->locked);
    mutex->locked = false;

    threadq_node_t *next_node = threadq_dequeue(&mutex->queue);
    if(next_node) scheduler_add_ready(next_node->thread);

    spinlock_unlock(&mutex->spinlock);
    interrupt_state_restore(interrupt_state);
}

#endif
