#ifndef LOCKS_H
#define LOCKS_H

#include <pthread.h>

void init_locks(); //to initialize read-write lock
void destroy_locks(); //to delete read-write lock
void read_lock(); //acquire read lock
void read_unlock(); //release read lock
void write_lock(); //acquire write lock
void write_unlock(); //release write lock

#endif