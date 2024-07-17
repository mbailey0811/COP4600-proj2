#include "hash_table.h"
#include "locks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Head of the linked list for the hash table
static hashRecord *head = NULL;

// Using the Jenkin's one at a time hash function as given
uint32_t hash(const uint8_t* key, size_t length) {
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

// Insert a key-value pair into the hash table
void insert(const char *key, uint32_t value) {
    // Calculate hash
    uint32_t hashValue = hash((const uint8_t*)key, strlen(key));   

    // Acquire write lock
    write_lock();

    hashRecord* temp = head;
    hashRecord* previous = NULL;

    // Look for node and update salary if already in list
    while (temp != NULL) {
        if (temp->hash == hashValue && strcmp(temp->name, key) == 0) {
            temp->salary = value;
            write_unlock();
            return;
        }
        // Move to next record
        previous = temp;
        temp = temp->next;
    }

    // If node isn't already in list, create new node
    hashRecord* newEntry = (hashRecord *)malloc(sizeof(hashRecord));
    
    // Null pointer check
    if (!newEntry) {
        printf("NULL POINTER, UNABLE TO INSERT");
        write_unlock();
        return;
    } 

    // Fill in the struct for the new entry
    newEntry->hash = hashValue;
    strcpy(newEntry->name, key);
    newEntry->salary = value;
    newEntry->next = NULL;

    // Previous should point to the end of the table,
    // so we just add the new entry next.
    // If previous doesn't, this must be the new head
    if (previous == NULL) {
        head = newEntry;
    } else {
        previous->next = newEntry;
    }

    // Release the write lock
    write_unlock();
}

// Delete a key-value pair from the hash table
void delete(const char *key) {
    // Calculate hash
    uint32_t hashValue = hash((const uint8_t*)key, strlen(key));   

    // Acquire write lock
    write_lock();

    hashRecord* temp = head;
    hashRecord* previous = NULL;

    // Look for node and update salary if already in list
    while (temp != NULL) {
        if (temp->hash == hashValue && strcmp(temp->name, key) == 0) {
            // Node has been found,
            // If it is first in the list then assign a new head
            if (previous == NULL) {
                head = temp->next;
            } else {
                // Otherwise, just skip this node in the list
                previous->next = temp->next;
            }
            // Free the memory and exit loop
            free(temp);
            break;
        }
        // Move to next record
        previous = temp;
        temp = temp->next;
    }
    write_unlock();
}

// Search for a key-value pair in the hash table
hashRecord* search(const char *key) {
    // Calculate hash
    uint32_t hashValue = hash((const uint8_t*)key, strlen(key));   

    // Acquire read lock
    read_lock();

    hashRecord* temp = head;

    // Look for node
    while (temp != NULL) {
        if (temp->hash == hashValue && strcmp(temp->name, key) == 0) {
            // Node has been found
            read_unlock();
            return temp;
        }
        // Move to next record
        temp = temp->next;
    }

    read_unlock();
    return NULL;
}


// added by Faramarz
// Function to get the head of the linked list
// This is used for testing purposes to iterate through the list
hashRecord* get_head() {
    return head;
}