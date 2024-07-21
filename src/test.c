// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include "hash_table.h"
// #include "locks.h"

// #define NUM_THREADS 5

// void *test_insert() {
//     char *names[] = {"Alice", "Bob", "Charlie", "David", "Eve"};
//     uint32_t salaries[] = {50000, 60000, 70000, 80000, 90000};

//     for (int i = 0; i < 5; i++) {
//         insert(names[i], salaries[i]);
//         printf("Inserted: %s with salary %u\n", names[i], salaries[i]);
//     }

//     return NULL;
// }

// void *test_search() {
//     char *names[] = {"Alice", "Bob", "Charlie", "David", "Eve"};

//     for (int i = 0; i < 5; i++) {
//         hashRecord *record = search(names[i]);
//         if (record) {
//             printf("Found: %s with salary %u\n", record->name, record->salary);
//         } else {
//             printf("No record found for %s\n", names[i]);
//         }
//     }

//     return NULL;
// }

// void *test_delete() {
//     char *names[] = {"Alice", "Bob", "Charlie", "David", "Eve"};

//     for (int i = 0; i < 5; i++) {
//         delete(names[i]);
//         printf("Deleted: %s\n", names[i]);
//     }

//     return NULL;
// }

// int main() {
//     pthread_t threads[NUM_THREADS];

//     // Initialize the locks
//     init_locks();

//     // Create threads to test insert
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, test_insert, NULL);
//     }

//     // Join threads
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     // Create threads to test search
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, test_search, NULL);
//     }

//     // Join threads
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     // Create threads to test delete
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, test_delete, NULL);
//     }

//     // Join threads
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     // Destroy the locks
//     destroy_locks();

//     return 0;
// }
