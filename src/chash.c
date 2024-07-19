#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "hash_table.h"
#include "locks.h"

#define MAX_COMMAND_LENGTH 256
#define MAX_THREADS 100

typedef struct {
    char command[MAX_COMMAND_LENGTH];
} Command;

HashTable *hash_table;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
int lock_acquisitions = 0;
int lock_releases = 0;

void log_operation(const char *operation) {
    pthread_mutex_lock(&file_mutex);
    FILE *output = fopen("output.txt", "a");
    fprintf(output, "%s\n", operation);
    fclose(output);
    pthread_mutex_unlock(&file_mutex);
}

void* process_command(void* arg) {
    Command *cmd = (Command *)arg;
    char operation[MAX_COMMAND_LENGTH];
    char action[MAX_COMMAND_LENGTH], name[MAX_COMMAND_LENGTH];
    int value, result;

    sscanf(cmd->command, "%[^,],%[^,],%d", action, name, &value);

    if (strcmp(action, "insert") == 0) {
        unsigned int hash = hash_table_insert(hash_table, name, value);
        sprintf(operation, "INSERT,%u,%s,%d", hash, name, value);
        log_operation(operation);
        log_operation("WRITE LOCK ACQUIRED");
        log_operation("WRITE LOCK RELEASED");
        lock_acquisitions++;
        lock_releases++;
    } else if (strcmp(action, "search") == 0) {
        unsigned int hash;
        if ((hash = hash_table_search(hash_table, name)) != -1) {
            sprintf(operation, "SEARCH,%s", name);
            log_operation(operation);
            sprintf(operation, "%u,%s,%d", hash, name, value);
            log_operation(operation);
        }
        log_operation("READ LOCK ACQUIRED");
        log_operation("READ LOCK RELEASED");
        lock_acquisitions++;
        lock_releases++;
    } else if (strcmp(action, "delete") == 0) {
        unsigned int hash = hash_table_delete(hash_table, name);
        if (hash != -1) {
            sprintf(operation, "DELETE,%s", name);
            log_operation(operation);
            sprintf(operation, "%u,%s,%d", hash, name, value);
            log_operation(operation);
        }
        log_operation("WRITE LOCK ACQUIRED");
        log_operation("WRITE LOCK RELEASED");
        lock_acquisitions++;
        lock_releases++;
    } else if (strcmp(action, "print") == 0) {
        log_operation("READ LOCK ACQUIRED");
        hash_table_print(hash_table);
        log_operation("READ LOCK RELEASED");
        lock_acquisitions++;
        lock_releases++;
    }
    return NULL;
}

int main() {
    FILE *file = fopen("commands.txt", "r");
    if (!file) {
        perror("Failed to open commands.txt");
        return 1;
    }

    hash_table = hash_table_create();

    char line[MAX_COMMAND_LENGTH];
    Command commands[MAX_THREADS];
    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    while (fgets(line, sizeof(line), file) && thread_count < MAX_THREADS) {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        strcpy(commands[thread_count].command, line);
        pthread_create(&threads[thread_count], NULL, process_command, (void *)&commands[thread_count]);
        thread_count++;
    }

    fclose(file);

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    log_operation("Number of lock acquisitions: " + lock_acquisitions);
    log_operation("Number of lock releases: " + lock_releases);

    hash_table_destroy(hash_table);
    pthread_mutex_destroy(&file_mutex);

    return 0;
}
