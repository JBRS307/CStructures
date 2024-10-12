#ifndef __THREAD_POOL
#define __THREAD_POOL

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef void (*ThreadFunc)(void* arg);

typedef struct ThreadPoolWorker ThreadPoolWorker;
struct ThreadPoolWorker {
    ThreadPoolWorker* next;
    ThreadFunc func;
    void* arg;
};

typedef struct {
    ThreadPoolWorker* head;
    ThreadPoolWorker* tail;
    pthread_mutex_t count_lock;
    pthread_cond_t work_cond; // Signaled when there is some work to do
    pthread_cond_t no_work_cond; // Signaled when there is NO work to do
    size_t total_threads;
    size_t working_threads;
    bool stop;
} ThreadPool;

#endif