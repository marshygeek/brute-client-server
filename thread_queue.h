#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

#include "declarations.h"

void queue_pop (queue_t * queue, task_t * task);

void queue_push (queue_t * queue, task_t * task);

void queue_init (queue_t * queue);

#endif // THREAD_QUEUE_H
