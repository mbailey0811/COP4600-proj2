/*
 Faramarz comment:
 
 this code is nothing but to test the insert, search, delete and locks functions
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "locks.h"



void test_insert_search_delete() {
    // Initialize locks
    init_locks();

    // Insert entries
    insert("Alice", 50000);
    insert("Bob", 60000);
    insert("Charlie", 70000);

    // Search for entries
    hashRecord* result = search("Alice");
    if (result != NULL) {
        printf("Found: %s, Salary: %u\n", result->name, result->salary);
    } else {
        printf("No Record Found for Alice\n");
    }

    result = search("Bob");
    if (result != NULL) {
        printf("Found: %s, Salary: %u\n", result->name, result->salary);
    } else {
        printf("No Record Found for Bob\n");
    }

    // Delete an entry
    delete("Alice");

    // Search for deleted entry
    result = search("Alice");
    if (result != NULL) {
        printf("Found: %s, Salary: %u\n", result->name, result->salary);
    } else {
        printf("No Record Found for Alice\n");
    }

    // Clean up locks
    destroy_locks();
}

int main() {
    test_insert_search_delete();
    return 0;
}