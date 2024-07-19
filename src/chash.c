#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "hash_table.h"
#include "locks.h"

#define MAX_COMMAND_LENGTH 256

extern FILE *output_file; // Declare the external variable
extern int num_lock_acquisitions; // Declare the external variable
extern int num_lock_releases; // Declare the external variable

int inserts_done = 0;
int insert_count = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;

void *process_command(void *arg) {
    char *command = (char *)arg;
    char *cmd = strtok(command, ",");
    char *param1 = strtok(NULL, ",");
    char *param2 = strtok(NULL, ",");

    if (strcmp(cmd, "insert") == 0) {
        insert(param1, atoi(param2));
        pthread_mutex_lock(&cond_mutex);
        inserts_done++;
        if (inserts_done == insert_count) {
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&cond_mutex);
    } else {
        pthread_mutex_lock(&cond_mutex);
        while (inserts_done < insert_count) {
            pthread_cond_wait(&cond, &cond_mutex);
        }
        pthread_mutex_unlock(&cond_mutex);

        if (strcmp(cmd, "delete") == 0) {
            delete(param1);
        } else if (strcmp(cmd, "search") == 0) {
            search(param1);
        } else if (strcmp(cmd, "print") == 0) {
            print_table();
        }
    }

    free(arg);
    return NULL;
}

int main() {
    printf("Initializing table...\n");
    initialize_table();

    printf("Opening commands.txt...\n");
    FILE *file = fopen("src/commands.txt", "r");
    if (file == NULL) {
        perror("Error opening commands.txt");
        return EXIT_FAILURE;
    }

    printf("Reading number of threads...\n");
    char command[MAX_COMMAND_LENGTH];
    fgets(command, sizeof(command), file);
    int num_threads = atoi(strtok(command, ",") + 8);  // Extract the number of threads
    pthread_t threads[num_threads];

    printf("Counting insert commands...\n");
    // Count the number of insert commands
    while (fgets(command, sizeof(command), file)) {
        if (strstr(command, "insert") != NULL) {
            insert_count++;
        }
    }

    // Rewind file to read commands again
    rewind(file);
    fgets(command, sizeof(command), file);  // Skip the first line again

    printf("Processing commands...\n");
    int i = 0;
    while (fgets(command, sizeof(command), file) && i < num_threads) {
        printf("Creating thread %d for command: %s", i, command);
        char *cmd_copy = strdup(command);
        if (pthread_create(&threads[i], NULL, process_command, (void *)cmd_copy) != 0) {
            perror("Error creating thread");
            return EXIT_FAILURE;
        }
        i++;
    }

    for (int j = 0; j < i; j++) {
        pthread_join(threads[j], NULL);  // Ensure threads are executed sequentially
    }

    fclose(file);

    fprintf(output_file, "Number of lock acquisitions: %d\n", num_lock_acquisitions);
    fprintf(output_file, "Number of lock releases: %d\n", num_lock_releases);
    print_table();
    fclose(output_file);

    printf("Finished processing commands.\n");
    return 0;
}
