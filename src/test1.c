/*
 Faramarz comment:
 
 this code is nothing but to test the insert, search, delete and locks functions
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "hash_table.h"
#include "locks.h"

// Number of threads to use in the test
#define NUM_THREADS 10

// Test function for concurrent insertions
void* thread_insert(void* arg) {
    char name[20];
    snprintf(name, sizeof(name), "User%d", *((int*)arg));
    printf("Thread %d Inserting: %s\n", *((int*)arg), name);
    insert(name, *((int*)arg) * 1000);
    return NULL;
}

// Test function for concurrent deletions
void* thread_delete(void* arg) {
    char name[20];
    snprintf(name, sizeof(name), "User%d", *((int*)arg));
    printf("Thread %d Deleting: %s\n", *((int*)arg), name);
    delete(name);
    return NULL;
}

// Test function for concurrent searches
void* thread_search(void* arg) {
    char name[20];
    snprintf(name, sizeof(name), "User%d", *((int*)arg));
    printf("Thread %d Searching: %s\n", *((int*)arg), name);
    hashRecord* result = search(name);
    if (result != NULL) {
        printf("Thread %d Found: %s, Salary: %u\n", *((int*)arg), result->name, result->salary);
    } else {
        printf("Thread %d No Record Found for %s\n", *((int*)arg), name);
    }
    return NULL;
}

void test_concurrent_operations() {
    // Initialize locks
    init_locks();

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Insert records concurrently
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_insert, &thread_ids[i]);
    }

    // Wait for all insert threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Search for records concurrently
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, thread_search, &thread_ids[i]);
    }

    // Wait for all search threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Delete records concurrently
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i + 1; // Reset thread ids to ensure correct names
        pthread_create(&threads[i], NULL, thread_delete, &thread_ids[i]);
    }

    // Wait for all delete threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Search for records again to confirm deletion
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, thread_search, &thread_ids[i]);
    }

    // Wait for all search threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Clean up locks
    destroy_locks();
}

int main() {
    test_concurrent_operations();
    return 0;
}











