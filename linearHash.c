#include "linearHash.h"

/*
FUNCTIONS:

    See rough algorithm in the textbook on page 381

*/


int hash(int level, int key) {
    level += 2;
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    return key;
}



void doubleBuckets(hashTable* ht) {
    int initialNumBuckets = ht->num_buckets;
    ht->num_buckets = ht->num_buckets * 2;
    ht->buckets = realloc(ht->buckets, sizeof(ridPage) * ht->num_buckets);

    for(int i = initialNumBuckets; i < ht->num_buckets; ++i) {
        ht->buckets[i] = (ridPage){.next = genRidPageptr(NULL), .prev = genRidPageptr(NULL), .nItems = 0};
        ht->buckets[i].next.type = -1; // this means that its not being used for overflow yet.
    }
}



int getNumBucketsAtLevel(int level) {
    return INITIAL_NUM_BUCKETS  * (1 << level); // N(level) = N(0) * 2 ^ level, level must be positive
}



void split(hashTable* ht) {
    ridPage bucketToBeSplit = *getPage(genRidPageptr(&ht->buckets[ht->next_index])).ptr.rid;
    
    ht->buckets[ht->next_index] = (ridPage){.next = genRidPageptr(NULL), .prev = genRidPageptr(NULL), .nItems = 0};
    ht->buckets[ht->next_index].next.type = -1; 
    
    ht->next_index++;
    // reinsert all the items in the bucket 
    for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
        rid recordID = bucketToBeSplit.rids[i];
        record r = recordID.page->records[recordID.slot];
        hashInsert(ht, r, ht->level+1);
    }

    // make sure to reinsert all of its overflow buckets
    while (bucketToBeSplit.next.type != -1) {
        ridPage* ptr = getPage(bucketToBeSplit.next).ptr.rid;
        bucketToBeSplit = *ptr;
        free(ptr);
        for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucketToBeSplit.rids[i];
            record r = recordID.page->records[recordID.slot];
            hashInsert(ht, r, ht->level+1);
        }
    }
    
    // increment level if next is pointing to the last bucket in that level also double buckets
    if (ht->next_index == getNumBucketsAtLevel(ht->level)) {
        doubleBuckets(ht);
        ht->level++;
        ht->next_index = 0;
    }

    return;
}



void hashInsert(hashTable* ht, record toAdd, int optionalLevel) {
    int key = toAdd.id;
    ridPage* bucket;
    int bucketIndex;
    if (optionalLevel == -1) {
        bucketIndex = hash(ht->level, key);
        
        if (bucketIndex < ht->next_index)
            bucketIndex = hash(ht->level+1, key);

        bucket = getPage(genRidPageptr(&ht->buckets[bucketIndex])).ptr.rid;
    }
    else {
        // use the argument-provided level
        bucketIndex = hash(optionalLevel, key);
        bucket = getPage(genRidPageptr(&ht->buckets[bucketIndex])).ptr.rid;
    }

    // is the bucket full?
    if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {
        if (optionalLevel == -1){
            // only split if you're not already splitting -- i.e. dont split if youre already in the processes of splitting 
            split(ht);
        }

        int bucketIndex = hash(ht->level, key);
        if (bucketIndex < ht->next_index) {
            bucketIndex = hash(ht->level+1, key);
        }
        bucket = getPage(genRidPageptr(&ht->buckets[bucketIndex])).ptr.rid;

        // the bucket has an overflow bucket if the type is not equal to -1. -1 implies that there is no overflow.
        while (bucket->next.type != -1) {
            bucket = getPage(bucket->next).ptr.rid;
        }

        // Do we need to set next to be an overflow bucket?
        if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {
            bucket->next = getPage(genRidPageptr(initRidPage())); // also sets next.type
            bucket->next.type = 0;
            bucket = bucket->next.ptr.rid;
            bucket->next.type = -1;       
        }    
    }

    rid recordID = addRecord(toAdd);
    bucket->rids[bucket->nItems] = recordID;
    bucket->nItems++;
    putPage(genRecordPageptr(recordID.page)); // record the write
    return;
}


record hashSearch(hashTable* ht, int key) {
    int bucketIndex = hash(ht->level, key);
    if (bucketIndex < ht->next_index) {
        bucketIndex = hash(ht->level+1, key);
    }

    // get main bucket -- record an ridpage read
    ridPage* bucket = getPage(genRidPageptr(&ht->buckets[bucketIndex])).ptr.rid;

    // get potential overflow buckets -- record an ridpage read for each overflow
    while (bucket->next.type != -1) {
        for (int i = 0; i < bucket->nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucket->rids[i];
            if (recordID.id == key) {
                return recordID.page->records[recordID.slot];
            }
        }
        
        bucket = getPage(bucket->next).ptr.rid;
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



recVec hashRangeSearch(hashTable* ht, int min, int max) {
    recVec ret = initRecVec();
    while (min <= max) {
        record r = hashSearch(ht, min);
        if (r.id != -1) recVecPush(&ret, r);
        min++;
    }
    return ret;
}


hashTable* initHashTable() {
    hashTable* ht = malloc(sizeof(hashTable)); // why do i have to do this instead of hashTable* ht;
    ht->level = 0; // set hash level to 2 (that is, we are going to be looking at the 2 least significant bits in the binary represntation of the key -- see hashing scheme)
    ht->num_buckets = INITIAL_NUM_BUCKETS*2; // the system only things there are 4 buckets to start, but because we double the number of buckets when next reaches the number of buckets at a given level, I need to start at 8
    ht->buckets = malloc(sizeof(ridPage) * ht->num_buckets);
    for(int i = 0; i < ht->num_buckets; ++i) {
        ht->buckets[i] = (ridPage){.next = genRidPageptr(NULL), .prev = genRidPageptr(NULL), .nItems = 0};
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
