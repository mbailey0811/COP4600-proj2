#ifndef LOCKS_H
#define LOCKS_H

#include <pthread.h>

typedef struct _rwlock_t {
    int readers;
    int writers;
    int waiting_writers;
    pthread_mutex_t mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
} rwlock_t;

void init_locks();        // Initialize read-write lock
void destroy_locks();     // Destroy read-write lock
void read_lock();         // Acquire read lock
void read_unlock();       // Release read lock
void write_lock();        // Acquire write lock
void write_unlock();      // Release write lock

#endif
