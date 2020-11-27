#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "pageManager.h"
#include "recVec.h"


#define INITIAL_NUM_BUCKETS 4 // This is is the initial number of buckets the hash table will start with.



typedef struct {
    int level; // initialized to zero, indicates the current round level and indicates which hash function to use
    int num_buckets;
    pageptr next; // initially to the first bucket. pointer to the bucket that will next be split
    int next_index; // index of the next pointer in the bucket array
    ridPage* buckets; //[INITIAL_NUM_BUCKETS]; // the actual table
} hashTable;

hashTable* initHashTable();

// generate records
record* genRandomRecords(int nRecords);

void doubleBuckets(hashTable* ht);
int hash(int level, int key);
void split(hashTable* ht);

void hashInsert(hashTable* ht, record toAdd, int optionalLevel);
record hashSearch(hashTable* ht, int key);
recVec hashRangeSearch(hashTable* ht, int min, int max);



/*
Christopher Chapman
Database Managment Systems (CPS-377)
Professor Brody
Final Project -- Fall 2020 (Module 2)
*/
