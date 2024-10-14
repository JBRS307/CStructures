#include <stdlib.h>
#include <errno.h>

#include "thread_pool.h"

static ThreadPoolError tp_errno = SUCCESS;

static void set_error(ThreadPoolError code) {
    tp_errno = code;
}

ThreadPoolError thread_pool_errno() {
    return tp_errno;
}

const char* thread_pool_error() {
    switch (tp_errno) {
        case SUCCESS:
            return "Success";
        case MEMORY_ALLOCATION_ERROR:
            return "Memory allocation error";
        case BUILTIN_ERROR:
            return "Builtin error (use perror)";
    }
}

static ThreadPoolWorker* tp_worker_create(ThreadFunc func, void* arg) {
    if (!func) {
        return NULL;
    }

    ThreadPoolWorker* new = (ThreadPoolWorker*)malloc(sizeof(ThreadPoolWorker));
    if (!new) {
        return NULL;
    }
    new->next = NULL;
    new->func = NULL;
    new->arg = arg;

    return new;
}

static void tp_worker_destroy(ThreadPoolWorker* work) {
    if (!work) {
        return;
    }
    free(work);
}

static ThreadPoolWorker* tp_worker_get(ThreadPool* tp) {
    if (!tp) {
        return NULL;
    }

    ThreadPoolWorker* work = tp->head;
    if (!work) {
        return NULL;
    }

    if (!work->next) {
        tp->head = tp->tail = NULL;
    } else {
        tp->head = work->next;
    }

    return work;
}

static void* tp_worker(void* arg) {
    ThreadPool* tp = (ThreadPool*)arg;
    if (!tp) {
        return;
    }

    while (true) {
        if ((errno = pthread_mutex_lock(&tp->count_lock)) != 0) {
            tp_errno = BUILTIN_ERROR;
            return;
        }

        while (!tp->head && !tp->stop) {
            if ((errno = pthread_cond_wait(&tp->work_cond, &tp->count_lock)) != 0) {
                tp_errno = BUILTIN_ERROR;
                return;
            }
        }

        if (tp->stop) {
            break;
        }

        ThreadPoolWorker* work = tp_worker_get(tp);
        tp->working_threads++;
        if ((errno = pthread_mutex_unlock(&tp->count_lock)) != 0) {
            tp_errno = BUILTIN_ERROR;
            return;
        }
        if (work) {
            work->func(work->arg);
        }
        if ((errno = pthread_mutex_lock(&tp->count_lock)) != 0) {
            tp_errno = BUILTIN_ERROR;
            return;
        }
        tp->working_threads--;
        if (!tp->stop && tp->working_threads == 0 && !tp->head) {
            if ((errno = pthread_cond_signal(&tp->no_work_cond)) != 0) {
                tp_errno = BUILTIN_ERROR;
                return;
            }
        }
        if ((errno = pthread_mutex_unlock(&tp->count_lock)) != 0) {
            tp_errno = BUILTIN_ERROR;
            return;
        }
    }
}

ThreadPool* thread_pool_create(size_t n) {
    if (n == 0) {
        n = 4;
    }

    ThreadPool* tp = (ThreadPool*)malloc(sizeof(ThreadPool));
    if (!tp) {
        tp_errno = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }
    tp->total_threads = n;

    if ((errno = pthread_mutex_init(&tp->count_lock, NULL)) != 0) {
        free(tp);
        tp_errno = BUILTIN_ERROR;
        return NULL;
    }
    if ((errno = pthread_cond_init(&tp->work_cond, NULL)) != 0) {
        pthread_mutex_destroy(&tp->count_lock);
        free(tp);
        tp_errno = BUILTIN_ERROR;
        return NULL;
    }
    if ((errno = pthread_cond_init(&tp->no_work_cond, NULL)) != 0) {
        pthread_mutex_destroy(&tp->count_lock);
        pthread_cond_destroy(&tp->work_cond);
        free(tp);
        tp_errno = BUILTIN_ERROR;
        return NULL;
    }

    pthread_t tid;
    for (size_t i = 0; i < n; i++) {
        if ((errno = pthread_create(&tid, NULL, tp_worker, tp)) != 0) {
            pthread_mutex_destroy(&tp->count_lock);
            pthread_cond_destroy(&tp->work_cond);
            pthread_cond_destroy(&tp->no_work_cond);
            free(tp);
            tp_errno = BUILTIN_ERROR;
            return NULL;
        }
        if ((errno = pthread_detach(tid)) != 0) {
            pthread_mutex_destroy(&tp->count_lock);
            pthread_cond_destroy(&tp->work_cond);
            pthread_cond_destroy(&tp->no_work_cond);
            free(tp);
            tp_errno = BUILTIN_ERROR;
            return NULL;
        }
    }
    tp_errno = SUCCESS;
    return tp;
}