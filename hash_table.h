#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>

//hash structure given in the instructions
typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

//Declare functions for hash_table.c:
// hash comes from the given Jenkins method
unt32_t hash(const char *key);

// insert, delete, search
// standard helper functions
void insert(const char *key, uint32_t value);
void delete(const char *key);
hashRecord* search(const char *key);


#endif