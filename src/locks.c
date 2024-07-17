#include <pthread.h>
#include "locks.h"
#include <stdio.h>

// Define the read-write lock
pthread_rwlock_t rwlock;

// Initialize the read-write lock
void init_locks() {
    pthread_rwlock_init(&rwlock, NULL);
}

// Destroy the read-write lock
void destroy_locks() {
    pthread_rwlock_destroy(&rwlock);
}

// Acquire a read lock
void read_lock() {
    printf("Read lock acquired\n");
    pthread_rwlock_rdlock(&rwlock);
}

// Release the read lock
void read_unlock() {
    printf("Read lock released\n");
    pthread_rwlock_unlock(&rwlock);
}

// Acquire a write lock
void write_lock() {
    printf("Write lock acquired\n");
    pthread_rwlock_wrlock(&rwlock);
}

// Release the write lock
void write_unlock() {
    printf("Write lock released\n");
    pthread_rwlock_unlock(&rwlock);
}
