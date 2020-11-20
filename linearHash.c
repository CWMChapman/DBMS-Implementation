#include "linearHash.h"

/*
FUNCTIONS:

    See rough algorithm in the textbook on page 381
    
    split
    delete
*/

int hash(int level, int key) {
    // printf("level: %i\n", level);
    // printf("before key: %i\n", key);
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    // printf("after key: %i\n", key);

    return key;
}


void insert(hashTable* ht, record toAdd) {
    int key = toAdd.id;
    int bucketIndex = hash(ht->level, key); // get the main bucket which the record should be stored in ... next check if there are overflow buckets
    ridPage* bucket = &ht->buckets[bucketIndex];

    // the bucket has an overflow bucket if the type is not equal to -1. -1 implies that there is no overflow.

    while (bucket->next.type != -1) {
        bucket = bucket->next.ptr.rid;
    }

    // Is the number of items == to the max size (capacity) of the bucket? if so, add overflow bucket
    // Add an overflow bucket because we've reached the capacity of current bucket
    if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {
        pageptr overflow_bucket = genRidPageptr(initRidPage());
        bucket->next = overflow_bucket;
        bucket = bucket->next.ptr.rid;
        bucket->next.type = -1;
    }

    rid recordID = addRecord(toAdd);
    int ridPageIndex = bucket->nItems;
    bucket->rids[ridPageIndex] = recordID;
    bucket->nItems++;
    putPage(genRecordPageptr(recordID.page));
    return;
}


record lookup(hashTable* ht, int key) {
    int bucketIndex = hash(ht->level, key);
    ridPage* bucket = &ht->buckets[bucketIndex];

    while (bucket->next.type != -1) {
        for (int i = 0; i < bucket->nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucket->rids[i];
            if (recordID.id == key) {
                return recordID.page->records[recordID.slot];
            }
        }
        bucket = bucket->next.ptr.rid;
    }
    for (int i = 0; i < bucket->nItems; ++i) {
        rid recordID = bucket->rids[i];
        if (recordID.id == key) {
            return recordID.page->records[recordID.slot];
        }
    }

    record empty_record = {.id = -1, .f1 = "EMPTY"}; // this means there was no matching key in the database 
    return empty_record;
}


hashTable* initHashTable() {
    hashTable* ht = malloc(sizeof(hashTable)); // why do i have to do this instead of hashTable* ht;
    ht->level = 2; // set hash level to 2 (that is, we are going to be looking at the 2 least significant bits in the binary represntation of the key -- see hashing scheme)
    ht->num_buckets = INITIAL_NUM_BUCKETS;
    for(int i = 0; i < ht->num_buckets; ++i) {
        ht->buckets[i] = *initRidPage();
        ht->buckets[i].next.type = -1; // this means that its not being used for overflow yet.
    }
    pageUnion ptr;
    ptr.rid = &ht->buckets[0];
    pageptr next; 
    next.type = 0;
    next.ptr = ptr;

    ht->next = next; // setting the next pointer (see linear hash scheme) to be the first bucket

    return ht;
}





// Code to test the hash_table functions
int main(int argc, char** argv) {
    initPageManager();
    hashTable* ht = initHashTable();
    // record r0 = {5, "five", "5"};
    // insert(ht, r0);
    for (int i = 0; i < 100; i++) {
        insert(ht, (record){i, "i", "empty"});
        printf("inserting: %d\n", i);
    }

    for (int i = 0; i < 100; i++) {
        record l = lookup(ht, i);
        printf("key: %d, value: %s\n", l.id, l.f1);
    }

    return 0;
}
