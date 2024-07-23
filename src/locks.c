#include "locks.h"
#include <time.h>

// Global variables
static rwlock_t lock;
static int locksAcquired = 0;
static int locksReleased = 0;

#define OUTPUT_FILE "output.txt"

//Function to get the current timestamp
long get_time() {
    time_t seconds = time(NULL);
    return (long)seconds;
}

//Function to get total number of locks acquired
int getLocksAcquired() {
    return locksAcquired;
}

//Function to get total number of locks released
int getLocksReleased() {
    return locksReleased;
}

// Function to log lock acquisition
void log_lock_acquired(const char *lock_type) {
    long timestamp = get_time();
    printf("%ld,%s LOCK ACQUIRED\n", timestamp, lock_type);
    FILE *output_file = fopen(OUTPUT_FILE, "a");
    if (output_file) {
        fprintf(output_file, "%ld,%s LOCK ACQUIRED\n", timestamp, lock_type);
        fclose(output_file);
    }
}

// Function to log lock release
void log_lock_released(const char *lock_type) {
    long timestamp = get_time();
    printf("%ld,%s LOCK RELEASED\n", timestamp, lock_type);
    FILE *output_file = fopen(OUTPUT_FILE, "a");
    if (output_file) {
        fprintf(output_file, "%ld,%s LOCK RELEASED\n", timestamp, lock_type);
        fclose(output_file);
    }
}

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
    locksAcquired++;
    while (lock.writers > 0 || lock.waiting_writers > 0) {
        // Wait if there are active or waiting writers
        pthread_cond_wait(&lock.read_cond, &lock.mutex);
    }
    lock.readers++;                              // Increment the number of readers
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
    log_lock_acquired("READ");
}

// Release a read lock
void read_unlock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    lock.readers--;                              // Decrement the number of readers
    locksReleased++;
    if (lock.readers == 0 && lock.waiting_writers > 0) {
        // Signal waiting writers if there are no more readers
        pthread_cond_signal(&lock.write_cond);
    }
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
    log_lock_released("READ");
}

// Acquire a write lock
void write_lock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    locksAcquired++;
    lock.waiting_writers++;                      // Increment the number of waiting writers
    while (lock.readers > 0 || lock.writers > 0) {
        // Wait if there are active readers or writers
        pthread_cond_wait(&lock.write_cond, &lock.mutex);
    }
    lock.waiting_writers--;                      // Decrement the number of waiting writers
    lock.writers++;                              // Increment the number of writers
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
    log_lock_acquired("WRITE");
}

// Release a write lock
void write_unlock() {
    pthread_mutex_lock(&lock.mutex);             // Acquire the mutex lock
    lock.writers--;                              // Decrement the number of writers
    locksReleased++;
    if (lock.waiting_writers > 0) {
        // Signal a waiting writer if any
        pthread_cond_signal(&lock.write_cond);
    } else {
        // Otherwise, broadcast to all waiting readers
        pthread_cond_broadcast(&lock.read_cond);
    }
    pthread_mutex_unlock(&lock.mutex);           // Release the mutex lock
    log_lock_released("WRITE");
}
