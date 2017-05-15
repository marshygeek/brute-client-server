#include "declarations.h"
#include "thread_queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

void queue_init (queue_t * queue){
    queue->head = 0;
    queue->tail = 0;
    pthread_mutex_init(&queue->head_mutex, NULL);
    pthread_mutex_init(&queue->tail_mutex, NULL);
    sem_init(&queue->full, 0, 0);
    sem_init(&queue->empty, 0, QUEUE_SIZE);
}

void queue_push (queue_t * queue, task_t * task){
    sem_wait(&queue->empty);

    pthread_mutex_lock(&queue->head_mutex);

    queue->queue[queue->head++] = *task;

    if(queue->head == QUEUE_SIZE)
        queue->head = 0;

    pthread_mutex_unlock(&queue->head_mutex);

    sem_post(&queue->full);
}

void queue_pop (queue_t * queue, task_t * task){
    sem_wait(&queue->full);

    pthread_mutex_lock(&queue->tail_mutex);

    *task = queue->queue[queue->tail++];

    if(queue->tail == QUEUE_SIZE)
        queue->tail = 0;

    pthread_mutex_unlock(&queue->tail_mutex);

    sem_post(&queue->empty);
}
