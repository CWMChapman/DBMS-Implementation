#include "linearHash.h"

/*
FUNCTIONS:

    See rough algorithm in the textbook on page 381
    delete

    Currently...

    I need to update lookup so it knows which hash scheme to use to find the bucket

    then i need to make delete

    make a random number generator so i can insert random keys so it doesnt become bottom heavy

    For benchmarking:
    https://github.com/CWMChapman/DBMS-Implementation/blob/8a2a08c29cd77fb5c751010f638bb5eaa8216c04/BPTNode.c



    Functions:

    search
    range search: look up each item and search rest of bucket
        record* rangeSearch(int min, int max)

    insert


    TODO: make sure im calling getPage and putPage!

    include print stats 




*/

record* genRandomRecords(int nRecords) {
    // generate in-order array
    record r;
    strcpy(r.f1, "i");
    strcpy(r.f2, "empty");
    record* ret = malloc(nRecords * sizeof(record));
    for (int i = 0; i < nRecords; ++i) {
        r.id = i;
        ret[i] = r;
    }

    // shuffle array--swap each spot with a different one
    record tmp;
    int randInt;
    srand(time(NULL));
    for (int i = 0; i < nRecords; ++i) {
        tmp = ret[i];
        randInt = rand() % nRecords;
        ret[i] = ret[randInt];
        ret[randInt] = tmp;
    }

    return ret;
}

int hash(int level, int key) {
    level += 2;
    int k = key;
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    printf("key: %d, hash: %d, level: %d\n", k, key, level);
    return key;
}

void doubleBuckets(hashTable* ht) {
    int initialNumBuckets = ht->num_buckets;
    ht->num_buckets = ht->num_buckets * 2;
    ht->buckets = realloc(ht->buckets, sizeof(ridPage) * ht->num_buckets);
    for(int i = initialNumBuckets; i < ht->num_buckets; ++i) {
        ht->buckets[i] = *initRidPage();
        ht->buckets[i].next.type = -1; // this means that its not being used for overflow yet.
    }
}

int getNumBucketsAtLevel(int level) {
    return INITIAL_NUM_BUCKETS  * (1 << level); // N(level) = N(0) * 2 ^ level, level must be positive
}

void split(hashTable* ht) {
    ridPage bucketToBeSplit = ht->buckets[ht->next_index];
    // free(&ht->buckets[ht->next_index]); // CHECK THIS FOR MEMORY LEAK!!!!!!!!!!
    ht->buckets[ht->next_index] = *initRidPage();
    ht->buckets[ht->next_index].next.type = -1; 
    

    // double buckets if this is the first time you've overflowed
    if (ht->next_index == 0) {
        doubleBuckets(ht);
    }

    // reinsert all the items in the bucket 
    for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
        rid recordID = bucketToBeSplit.rids[i];
        record r = recordID.page->records[recordID.slot];
        insert(ht, r, ht->level+1);
    }
    // make sure to reinsert all of its overflow buckets
    while (bucketToBeSplit.next.type != -1) {
        ridPage* ptr = bucketToBeSplit.next.ptr.rid;
        bucketToBeSplit = *ptr;
        free(ptr);
        for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucketToBeSplit.rids[i];
            record r = recordID.page->records[recordID.slot];
            insert(ht, r, ht->level+1);
        }
    }

    // increment level if next is pointing to the last bucket in that level also double buckets
    if (ht->next_index == getNumBucketsAtLevel(ht->level)-1) {
        printf("\n\n\nDOUBLING!!!! \t ht->next_index: %d, getNumBucketsAtLevel(%d)-1: %d\n\n\n", ht->next_index, ht->level, getNumBucketsAtLevel(ht->level)-1);
        doubleBuckets(ht);
        ht->level++;
        ht->next_index = 0;
    }
    else {
        ht->next_index++;
    }

    return;
}



void insert(hashTable* ht, record toAdd, int optionalLevel) {
    int key = toAdd.id;
    ridPage* bucket;
    if (optionalLevel == -1) {
        // use ht's level
        int bucketIndex = hash(ht->level, key);
        
        if (bucketIndex < ht->next_index) {
            // printf("\n not OPTIONAL ARGUMENT\n");
            bucketIndex = hash(ht->level+1, key);
        }

        bucket = &ht->buckets[bucketIndex];
    }
    else {
        // use the argument-provided level
        int bucketIndex = hash(optionalLevel, key);
        bucket = &ht->buckets[bucketIndex];
    }

    // Is the number of items == to the max size (capacity) of the bucket? if so, add overflow bucket
    //If you add an overflow bucket, call split so that it can split the bucket for the next pointer and increment the level
    if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {
        split(ht);
        insert(ht, toAdd, -1);
        // return;
        // bucket->next = getPage(genRidPageptr(initRidPage())); // set next to be an overflow bucket
        // bucket->next.type = -1;       
        // getPage(bucket->next); 
        // bucket = bucket->next.ptr.rid;
        printf("\nSPLITTING!!!!\n");
    }

    // the bucket has an overflow bucket if the type is not equal to -1. -1 implies that there is no overflow.
    while (bucket->next.type != -1) {
        getPage(bucket->next);
        bucket = bucket->next.ptr.rid;
    }

    rid recordID = addRecord(toAdd);
    int ridPageIndex = bucket->nItems;
    bucket->rids[ridPageIndex] = recordID;
    bucket->nItems++;
    putPage(genRecordPageptr(recordID.page)); // record the write
    return;
}


record search(hashTable* ht, int key) {
    int bucketIndex = hash(ht->level, key);
    if (bucketIndex < ht->next_index) {
        bucketIndex = hash(ht->level+1, key);
    }

    printf("key: %d, bucketIndex = %d\n", key, bucketIndex);
    ridPage* bucket = &ht->buckets[bucketIndex];
    // printf("bucket->next.type = %d\n", bucket->next.type);
    while (bucket->next.type != -1) {
        for (int i = 0; i < bucket->nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucket->rids[i];
            if (recordID.id == key) {
                getPage(getPage(genRidPageptr(initRidPage())));
                return recordID.page->records[recordID.slot];
            }
        }
        bucket = bucket->next.ptr.rid;
    }
    for (int i = 0; i < bucket->nItems; ++i) {
        rid recordID = bucket->rids[i];
        if (recordID.id == key) {
            getPage(getPage(genRidPageptr(initRidPage())));
            return recordID.page->records[recordID.slot];
        }
    }

    record empty_record = {.id = -1, .f1 = "EMPTYyyyyyyyyyyyyyy"}; // this means there was no matching key in the database 
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

    
    int n = 200;
    
    record* rArray = genRandomRecords(n);
    for (int i = 0; i < n; i++) {
        insert(ht, rArray[i], -1);
    }

    // search n key-value pairs
    printf("\n\n\n\n\n\n\n");
    for (int i = 0; i < n; i++) {
        record l = search(ht, i);
        printf("found key: %d, value: %s\n", l.id, l.f1);
    }




    printf("the number of buckets: %d, level: %d\n", ht->num_buckets, ht->level);
    for (int i = 0; i < ht->num_buckets; ++i) {
        printf("buckets[%d].nItems = %d\n", i, ht->buckets[i].nItems);
    }
    printf("next index: %d\n", ht->next_index);
    printf("getNumBucketsAtLevel(%d)-1 = %d\n", ht->level, getNumBucketsAtLevel(ht->level)-1);
    printPageStats();

    return 0;
}
