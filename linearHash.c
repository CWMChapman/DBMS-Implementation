#include "linearHash.h"
#include <math.h>
/*
FUNCTIONS:

    See rough algorithm in the textbook on page 381
    
    split
    delete

    Currently...

    Split works, it doubles the buckets, and reinserts all the elements in the bucket 
    being split. 

    The problem currently is that insert just inserts the elements back into the first bucket
    which is what happened before we split, because it doesnt recognize that the level has changed.







*/

int hash(int level, int key) {
    level += 2;
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    return key;
}

void split(hashTable* ht) {
    ridPage bucketToBeSplit = ht->buckets[ht->next_index];
    // free(&ht->buckets[ht->next_index]); // CHECK THIS FOR MEMORY LEAK!!!!!!!!!!
    ht->buckets[ht->next_index] = *initRidPage();
    ht->buckets[ht->next_index].next.type = -1; 

    printf("bucketToBeSplit: %d\n", bucketToBeSplit.nItems);
    
    ht->num_buckets = ht->num_buckets * (1 << (ht->level+1));
    ht->buckets = realloc(ht->buckets, sizeof(ridPage) * ht->num_buckets);
    while (bucketToBeSplit.next.type != -1) {
        for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucketToBeSplit.rids[i];
            record r = recordID.page->records[recordID.slot];
            insert(ht, r);
        }
        bucketToBeSplit = *bucketToBeSplit.next.ptr.rid;
    }
    for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
        rid recordID = bucketToBeSplit.rids[i];
        record r = recordID.page->records[recordID.slot];
        insert(ht, r);
    }
    ht->next_index++;
    ht->level++;
    return;
}

int getNumBucketsAtLevel(int level) {
    return INITIAL_NUM_BUCKETS  * (1 << level); // N(level) = N(0) * 2 ^ level, level must be positive
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
    //If you add an overflow bucket, call split so that it can split the bucket for the next pointer and increment the level
    if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {

        // split(ht);
        // ht->level++;
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
    ht->level = 0; // set hash level to 2 (that is, we are going to be looking at the 2 least significant bits in the binary represntation of the key -- see hashing scheme)
    ht->num_buckets = INITIAL_NUM_BUCKETS; // * pow(2,ht->level); <- when level zero its just 4
    ht->buckets = malloc(sizeof(ridPage) * ht->num_buckets);
    for(int i = 0; i < ht->num_buckets; ++i) {
        ht->buckets[i] = *initRidPage();
        ht->buckets[i].next.type = -1; // this means that its not being used for overflow yet.
    }
    pageUnion ptr;
    ptr.rid = &ht->buckets[0];
    ht->next_index = 0;
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

    split(ht);
    for (int i = 0; i < ht->num_buckets; ++i) {
        printf("buckets[%d].nItems = %d\n", i, ht->buckets[i].nItems);
    }

    return 0;
}
