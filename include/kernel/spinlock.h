#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <arch/cpu.h>

typedef struct spinlock_t
{
    int value;
} spinlock_t;

#define SPINLOCK_INIT { 0 }

static inline spinlock_t* spinlock_init(spinlock_t *spinlock)
{
    spinlock->value = 0;
    return spinlock;
}

static inline void spinlock_lock(spinlock_t *spinlock)
{
    while(!__sync_bool_compare_and_swap(&spinlock->value, 0, 1))
        cpu_pause();
}

static inline void spinlock_unlock(spinlock_t *spinlock)
{
    while(!__sync_bool_compare_and_swap(&spinlock->value, 1, 0))
        cpu_pause();
}

#endif
