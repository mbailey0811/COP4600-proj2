#include "hash_table.h"
#include "locks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_LINE_LENGTH 256
#define INPUT_FILE "commands.txt"
#define OUTPUT_FILE "output.txt"

// Function to execute commands from a file
void execute_commands(const char *filename);

// Function to get the current timestamp
long get_timestamp() {
    time_t seconds = time(NULL);
    return (long)seconds;
}

//Function to be used as comparator for qsort
int hashComp(const void *a, const void *b) {
    hashRecord *hashA = *(hashRecord**)a;
    hashRecord *hashB = *(hashRecord**)b;

    if (hashA->hash > hashB->hash) {
        return 1;
    } else if (hashA->hash == hashB->hash) {
        return 0;
    } else {
        return -1;
    }
}

//Function to print the final hash table
void print_table(FILE *output) {
    read_lock();

    //iterate to end of list to count entries
    int numRecords = 0;
    hashRecord *temp = get_head();
    while (temp != NULL) {
        numRecords++;
        temp = temp->next;
    }

    hashRecord **hashArr = malloc(numRecords * sizeof(hashRecord*));
    if (hashArr == NULL) {
        //malloc fail
        read_unlock();
        return;
    }

    //copy to array
    temp = get_head();
    for (int i=0; i<numRecords; i++) {
        hashArr[i] = temp;
        temp = temp->next;
    }

    //sort by hash to print in hash order
    qsort(hashArr, numRecords, sizeof(hashRecord*), hashComp);

    for (int i=0; i<numRecords; i++) {
        fprintf(output, "%u,%s,%u\n", hashArr[i]->hash, hashArr[i]->name, hashArr[i]->salary);
    }

    //clean up
    free(hashArr);
    read_unlock();
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n CHash is hardcoded to read from commands.txt!\n", argv[0]);
        return 1;
    }

    execute_commands(INPUT_FILE);

    //open file again for final table print
    FILE* output = fopen(OUTPUT_FILE, "a");
    if (output == NULL) {
        printf("Unable to open file");
        return 1;
    }

    fprintf(output, "\n");
    fprintf(output, "Number of locks acquisitions: %d\n", getLocksAcquired());
    fprintf(output, "Number of lock releases: %d\n", getLocksReleased());
    print_table(output);
    fclose(output);
    return 0;
}

void execute_commands(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    printf("Opened commands file: %s\n", filename);

    FILE *output_file = fopen(OUTPUT_FILE, "w");
    if (!output_file) {
        perror("Unable to open output file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    printf("Opened output file: %s\n", OUTPUT_FILE);

    char line[MAX_LINE_LENGTH];
    int num_threads = 1; // Default value

    // Read the number of threads from the first line
    if (fgets(line, sizeof(line), file)) {
        printf("Read line: %s", line);
        
        // Adjust sscanf to only capture the number after "threads,"
        int parsed = sscanf(line, "threads,%d", &num_threads);
        if (parsed != 1) {
            fprintf(stderr, "Invalid or missing threads command: %s", line);
            fclose(file);
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

        printf("Running %d threads\n", num_threads);
        fprintf(output_file, "Running %d threads\n", num_threads);
        fflush(output_file); // Ensure data is written to the file

        // Initialize hash table with the number of threads if needed
        // initialize_hash_table(num_threads); // Commented out or removed if not needed
    } else {
        fprintf(stderr, "Failed to read the number of threads.\n");
        fclose(file);
        fclose(output_file);
        exit(EXIT_FAILURE);
    }

    //Initialize the locks
    init_locks();

    // Process remaining commands
    while (1) {
        if (!fgets(line, sizeof(line), file)) {
            break;
        }
        printf("Read line: %s", line);
        
        char command[MAX_LINE_LENGTH];
        char key[MAX_LINE_LENGTH];
        uint32_t value = 0;  // Initialize value to 0 by default
        int parsed = sscanf(line, "%[^,],%[^,],%u", command, key, &value);
        if (parsed < 2) {  // Only require command and key, value is optional
            printf("Failed to parse line: %s", line);
            continue;
        }

        printf("Parsed command: %s, key: %s, value: %u\n", command, key, value);

        long timestamp = get_timestamp();
        printf("Timestamp: %ld\n", timestamp);

        if (strcmp(command, "insert") == 0) {
            printf("Inserting: %s with value: %u\n", key, value);
            insert(key, value);
            printf("Inserted: %s with value: %u\n", key, value);
            fprintf(output_file, "%ld,INSERT,%s,%u\n", timestamp, key, value);
            fflush(output_file); // Ensure data is written to the file
        } else if (strcmp(command, "delete") == 0) {
            printf("Deleting: %s\n", key);
            delete(key);
            fprintf(output_file, "%ld,DELETE,%s,0\n", timestamp, key);
            fflush(output_file); // Ensure data is written to the file
            printf("Deleted: %s\n", key);
        } else if (strcmp(command, "search") == 0) {
            printf("Searching: %s\n", key);
            hashRecord *result = search(key);
            if (result) {
                fprintf(output_file, "%ld,SEARCH,%s,%u\n", timestamp, key, result->salary);
                fflush(output_file); // Ensure data is written to the file
                printf("Found: %s with value: %u\n", key, result->salary);
            } else {
                fprintf(output_file, "%ld,SEARCH,%s,0\n", timestamp, key);
                fflush(output_file); // Ensure data is written to the file
                printf("No Record Found for: %s\n", key);
            }
        } else {
            fprintf(output_file, "%ld,UNKNOWN,%s,0\n", timestamp, command);
            fflush(output_file); // Ensure data is written to the file
            printf("Unknown command: %s\n", command);
        }
    }

    //Destroy the lcoks
    destroy_locks();

    fclose(file);
    fclose(output_file);
    printf("Finished processing commands.\n");
}
