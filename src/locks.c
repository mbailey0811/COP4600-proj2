#include "locks.h"

// Global lock variable
static rwlock_t lock;

// Initialize the reader-writer lock
void init_locks() {
    lock.readers = 0;
    lock.writers = 0;
    lock.waiting_writers = 0;
    pthread_mutex_init(&lock.mutex, NULL);       // Initialize the mutex
    pthread_cond_init(&lock.read_cond, NULL);    // Initialize the reader condition variable
    pthread_cond_init(&lock.write_cond, NULL);   // Initialize the writer condition variable
}

// Destroy the reader-writer lock
void destroy_locks() {
    pthread_mutex_destroy(&lock.mutex);          // Destroy the mutex
    pthread_cond_destroy(&lock.read_cond);       // Destroy the reader condition variable
    pthread_cond_destroy(&lock.write_cond);      // Destroy the writer condition variable
}

// Acquire a read lock
void read_lock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    while (lock.writers > 0 || lock.waiting_writers > 0) {
        // Wait if there are active or waiting writers
        pthread_cond_wait(&lock.read_cond, &lock.mutex);
    }
    lock.readers++;                              // Increment the number of readers
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
}

// Release a read lock
void read_unlock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    lock.readers--;                              // Decrement the number of readers
    if (lock.readers == 0 && lock.waiting_writers > 0) {
        // Signal waiting writers if there are no more readers
        pthread_cond_signal(&lock.write_cond);
    }
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
}

// Acquire a write lock
void write_lock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    lock.waiting_writers++;                      // Increment the number of waiting writers
    while (lock.readers > 0 || lock.writers > 0) {
        // Wait if there are active readers or writers
        pthread_cond_wait(&lock.write_cond, &lock.mutex);
    }
    lock.waiting_writers--;                      // Decrement the number of waiting writers
    lock.writers++;                              // Increment the number of writers
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
}

// Release a write lock
void write_unlock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    lock.writers--;                              // Decrement the number of writers
    if (lock.waiting_writers > 0) {
        // Signal a waiting writer if any
        pthread_cond_signal(&lock.write_cond);
    } else {
        // Otherwise, broadcast to all waiting readers
        pthread_cond_broadcast(&lock.read_cond);
    }
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
}
