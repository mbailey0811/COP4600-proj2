#include "hash_table.h"
#include "locks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//head of the linked list for the hash table
static hashRecord *head = NULL;

//using the Jenkin's one at a time hash function as given
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

void insert(const char *key, uint32_t value) {
    //calculate hash
    uint32_t hashValue = hash(key, strlen(key));   

    //acquire lock
    write_lock();

    hashRecord* temp = head;
    hashRecord* previous = NULL;

    //look for node and update salary if already in list
    while (temp != NULL) {
        if (temp->hash == hashValue && strcmp(temp->name, key) == 0) {
            temp->salary = value;
            write_unlock();
            return;
        }
        //move to next record
        previous = temp;
        temp = temp->next;
    }

    //if node isn't already in list, create new node
    hashRecord* newEntry = (hashRecord *)malloc(sizeof(hashRecord));
    
    //null pointer check
    if (!newEntry) {
        printf("NULL POINTER, UNABLE TO INSERT");
        return;
    } 

    //fill in the struct for the new entry
    newEntry->hash = hashValue;
    strcpy(newEntry->name, key);
    newEntry->salary = value;
    newEntry->next = NULL;

    //previous should point to the end of the table,
    //so we just add the new entry next.
    //if previous doesn't, this must be the new head
    if (previous == NULL) {
        head = newEntry;
    } else {
        previous->next = newEntry;
    }

    //release the write lock
    write_unlock();
}

void delete(const char *key) {
    //calculate hash
    uint32_t hashValue = hash(key, strlen(key));   

    //acquire lock
    write_lock();

    hashRecord* temp = head;
    hashRecord* previous = NULL;

    //look for node and update salary if already in list
    while (temp != NULL) {
        if (temp->hash == hashValue && strcmp(temp->name, key) == 0) {
            //node has been found,
            //if it is first in the list then assign a new head
            if (previous == NULL) {
                head = temp->next;
            } else {
                //otherwise, just skip this node in the list
                previous->next = temp->next;
            }
            //free the memory and exit loop
            free(temp);
            break;
        }
        //move to next record
        previous = temp;
        temp = temp->next;
    }
    write_unlock();
}

hashRecord* search(const char *key) {
    //calculate hash
    uint32_t hashValue = hash(key, strlen(key));   

    //acquire lock
    read_lock();

    hashRecord* temp = head;

    //look for node
    while (temp != NULL) {
        if (temp->hash == hashValue && strcmp(temp->name, key) == 0) {
            //node has been found
            read_unlock();
            return temp;
        }
        //move to next record
        temp = temp->next;
    }

    read_unlock();
    return NULL;

}