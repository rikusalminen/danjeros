#ifndef CONDITION_H
#define CONDITION_H

#include <kernel/spinlock.h>
#include <kernel/thread_queue.h>
#include <kernel/scheduler.h>
#include <kernel/mutex.h>

typedef struct condition_t
{
    spinlock_t spinlock;
    threadq_t queue;
} condition_t;

#define CONDITION_INIT { .spinlock = SPINLOCK_INIT, .queue = THREADQ_INIT }

static inline condition_t* condition_init(condition_t *condition)
{
    spinlock_init(&condition->spinlock);
    threadq_init(&condition->queue);
    return condition;
}

static inline void condition_wait(condition_t *condition, mutex_t *mutex)
{
    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&condition->spinlock);

    mutex_unlock(mutex);

    thread_t *this_thread = scheduler_current_thread();
    threadq_enqueue(&condition->queue, &this_thread->queue_node);

    spinlock_unlock(&condition->spinlock);
    scheduler_yield(false);
    interrupt_state_restore(interrupt_state);

    mutex_lock(mutex);
}

static inline void condition_signal(condition_t *condition)
{
    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&condition->spinlock);

    threadq_node_t *next_node = threadq_dequeue(&condition->queue);
    if(next_node) scheduler_add_ready(next_node->thread);

    spinlock_unlock(&condition->spinlock);
    interrupt_state_restore(interrupt_state);
}

static inline void condition_broadcast(condition_t *condition)
{
    interrupt_state_t interrupt_state = interrupt_disable();
    spinlock_lock(&condition->spinlock);

    scheduler_add_queue(&condition->queue);

    spinlock_unlock(&condition->spinlock);
    interrupt_state_restore(interrupt_state);
}

#endif
