#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>

typedef struct threadq_t threadq_t;
typedef struct thread_t thread_t;

void scheduler_init();
void scheduler_yield(bool ready);
void scheduler_yield_interrupt(registers_t *registers, interrupt_stack_frame_t *interrupt_stack_frame, bool ready);
void scheduler_add_ready(thread_t *thread);
void scheduler_add_queue(threadq_t *queue);

#endif
