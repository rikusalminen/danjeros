#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

#include <stddef.h>

#include <assert.h>

typedef struct thread_t thread_t;

typedef struct threadq_node_t
{
    thread_t *thread;
    struct threadq_node_t *next, *prev;
} threadq_node_t;

static inline threadq_node_t *threadq_node_init(threadq_node_t *node, thread_t *thread)
{
    assert(node);
    assert(thread);

    node->next = node->prev = NULL;
    node->thread = thread;
    return node;
}

typedef struct threadq_t
{
    threadq_node_t *first, *last;
} threadq_t;

static inline threadq_t* threadq_init(threadq_t *queue)
{
    queue->first = queue->last = NULL;
    return queue;
}

static inline void threadq_enqueue(threadq_t *queue, threadq_node_t *node)
{
    assert(queue);
    assert(node);
    assert((node->next == NULL) && (node->prev == NULL));
    assert((queue->first == NULL) == (queue->last == NULL));

    if(queue->first == NULL) queue->first = queue->last = node;
    else
    {
        assert(queue->last->next == NULL);
        node->prev = queue->last;
        node->prev->next = node;
        queue->last = node;
    }
}

static inline threadq_node_t *threadq_dequeue(threadq_t *queue)
{
    assert(queue);
    assert((queue->first == NULL) == (queue->last == NULL));

    if(queue->first == NULL) return NULL;

    threadq_node_t *node = queue->first;
    if(queue->last == node) queue->last = NULL;
    queue->first = node->next;
    if(queue->first != NULL) queue->first->prev = NULL;

    node->next = node->prev = NULL;

    return node;
}

static inline void threadq_remove(threadq_t *queue, threadq_node_t *node)
{
    assert(queue);
    assert(node);

    if(queue->first == node) queue->first = node->next;
    if(queue->last == node) queue->last = node->prev;

    threadq_node_t *prev = node->prev;
    if(node->prev) node->prev->next = node->next;
    if(node->next) node->next->prev = prev;

    node->next = node->prev = NULL;
}

static inline void threadq_splice(threadq_t *to, threadq_t *from)
{
    assert(from);
    assert(to);
    assert((from->first == NULL) == (from->last == NULL));
    assert((to->first == NULL) == (to->last == NULL));

    if(from->first == NULL) return;
    if(to->first == NULL)
    {
        to->first = from->first;
        to->last = from->last;
        from->first = from->last = NULL;
        return;
    }

    assert(to->last->next == NULL && from->first->prev == NULL);
    assert(to->first->prev == NULL && from->last->next == NULL);

    to->last->next = from->first;
    from->first->prev = to->last;

    to->last = from->last;
    from->first = from->last = NULL;
}

#endif
