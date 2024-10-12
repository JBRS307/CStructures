#include <stdlib.h>
#include <errno.h>

#include "thread_pool.h"

ThreadPool* thread_pool_create(size_t n) {
    ThreadPool* tp = (ThreadPool*)malloc(sizeof(ThreadPool));
    
}