#include <string.h>
#include <math.h>
#include <time.h>

#include "pageManager.h"


#define INITIAL_NUM_BUCKETS 4 // This is is the initial number of buckets the hash table will start with.



typedef struct {
    int level; // initialized to zero, indicates the current round level and indicates which hash function to use
    int num_buckets;
    pageptr next; // initially to the first bucket. pointer to the bucket that will next be split
    int next_index; // index of the next pointer in the bucket array
    ridPage* buckets; //[INITIAL_NUM_BUCKETS]; // the actual table
} hashTable;

typedef struct {
    int size;
    int array[10000];
} randArray;

randArray* initRandArray();
int randInt(int upper_bound); // implicit lower bound of 0
int getRandKey(randArray* rA);


hashTable* initHashTable();

void doubleBuckets(hashTable* ht);
void split(hashTable* ht);
int hash(int level, int key);
void insert(hashTable* ht, record toAdd, int optionalLevel);
record lookup(hashTable* ht, int key, int optionalLevel);

/*
Christopher Chapman
Database Managment Systems (CPS-377)
Professor Brody
Final Project -- Fall 2020 (Module 2)
*/
