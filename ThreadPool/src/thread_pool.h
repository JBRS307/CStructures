#ifndef __THREAD_POOL
#define __THREAD_POOL

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef enum {
    SUCCESS,
    MEMORY_ALLOCATION_ERROR,
    BUILTIN_ERROR,
} ThreadPoolError;

typedef void (*ThreadFunc)(void* arg);

// Returns most recent error number
ThreadPoolError thread_pool_errno();

// Returns message of most recent error
const char* thread_pool_error();

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

// Dynamically initializes ThreadPool with n threads
ThreadPool* thread_pool_create(size_t n);


#endif