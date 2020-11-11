#include <string.h>
// #include<time.h>

#include "pageManager.h"

#define INITIAL_NUM_BUCKETS 4 // This is is the initial number of buckets the hash table will start with.



typedef struct {
    int level; // initialized to zero, indicates the current round level and indicates which hash function to use
    ridPage* next; // initially to the first bucket. pointer to the bucket that will next be split
    ridPage buckets[(INITIAL_NUM_BUCKETS * sizeof(struct ridPage*)) / sizeof(ridPage)]; // the actual table
} hash_table;

/*
Christopher Chapman
Database Managment Systems (CPS-377)
Professor Brody
Final Project -- Fall 2020 (Module 2)
*/
