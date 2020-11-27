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



    LOOK AT EVERY TIME I MAKE A NEW BUCKET AND *******MAKE******* SURE THAT I AM SETTTING NEXT TO BE NEGATIVE 1



*/



record* genRandomRecords(int nRecords) {
    // generate in-order array
    record r;
    strcpy(r.f1, "f1");
    strcpy(r.f2, "f2");
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
    ridPage bucketToBeSplit = ht->buckets[ht->next_index];

    ht->buckets[ht->next_index] = (ridPage){.next = genRidPageptr(NULL), .prev = genRidPageptr(NULL), .nItems = 0};
    ht->buckets[ht->next_index].next.type = -1; 
    
    // reinsert all the items in the bucket 
    for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
        rid recordID = bucketToBeSplit.rids[i];
        getPage(genRecordPageptr(recordID.page));
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
            getPage(genRecordPageptr(recordID.page));
            record r = recordID.page->records[recordID.slot];
            insert(ht, r, ht->level+1);
        }
    }

    // increment level if next is pointing to the last bucket in that level also double buckets
    if (ht->next_index == getNumBucketsAtLevel(ht->level)-1) {
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
        int bucketIndex = hash(ht->level, key);
        
        if (bucketIndex < ht->next_index)
            bucketIndex = hash(ht->level+1, key);

        bucket = &ht->buckets[bucketIndex];
    }
    else {
        // use the argument-provided level
        int bucketIndex = hash(optionalLevel, key);
        bucket = &ht->buckets[bucketIndex];
    }

    // is the bucket full?
    if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {
        split(ht);

        int bucketIndex = hash(ht->level, key);
        if (bucketIndex < ht->next_index) {
            bucketIndex = hash(ht->level+1, key);
        }
        bucket = &ht->buckets[bucketIndex];

        // the bucket has an overflow bucket if the type is not equal to -1. -1 implies that there is no overflow.
        while (bucket->next.type != -1) {
            getPage(bucket->next);
            bucket = bucket->next.ptr.rid;
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


record search(hashTable* ht, int key) {
    int bucketIndex = hash(ht->level, key);
    if (bucketIndex < ht->next_index) {
        bucketIndex = hash(ht->level+1, key);
    }

    ridPage* bucket = &ht->buckets[bucketIndex];
    while (bucket->next.type != -1) {
        for (int i = 0; i < bucket->nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucket->rids[i];
            if (recordID.id == key) {
                getPage(genRecordPageptr(recordID.page));
                return recordID.page->records[recordID.slot];
            }
        }
        bucket = bucket->next.ptr.rid;
    }
    for (int i = 0; i < bucket->nItems; ++i) {
        rid recordID = bucket->rids[i];
        if (recordID.id == key) {
            getPage(genRecordPageptr(recordID.page));
            return recordID.page->records[recordID.slot];
        }
    }

    record empty_record = {.id = -1, .f1 = "EMPTY"}; // this means there was no matching key in the database 
    return empty_record;
}



recVec rangeSearch(hashTable* ht, int min, int max) {
    if (max == -1) {
        max = INT_MAX;
    }
    recVec ret = initRecVec();
// recVecPush(&ret, curRecord);
    while (min <= max) {
        record r = search(ht, min);
        if (r.id != -1) {
            recVecPush(&ret, r);
        }
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


// Code to test the hash_table functions
int main(int argc, char** argv) {
    initPageManager();
    hashTable* ht = initHashTable();
    
    int n = 1800000; //799970
    printf("\n\nInserting %d records\n", n);
    clock_t t; 
    t = clock(); 


    /* *** HASH TABLE CODE IS RUN HERE *** */
    record* rArray = genRandomRecords(n);

    // insert
    for (int i = 0; i < n; i++) {
        insert(ht, rArray[i], -1);
    }
    printf("\nInsert\n");
    printPageStats();
    clearPageManager();

    // search
    for (int i = 0; i < n; i++) {
        record l = search(ht, i);
        if(l.id == -1)
            printf("ERROR!! %d\n", i);
    }
    printf("\nSearch\n");
    printPageStats();
    clearPageManager();

    // range search
    recVec v = rangeSearch(ht, 5, 10);
    printRecVec(v); 
    printf("\nRange Search\n");
    printPageStats();
    clearPageManager();

    printf("Finished search!\n\n");
    /* ****** ^^ HASH TABLE CODE ^^ ****** */

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("HashTable with %d records (insert-search) took %f seconds to execute\n", n, time_taken);

    printf("there are %d buckets in the hash table\n", ht->num_buckets);
    
    // for (int i = 0; i < ht->num_buckets; i++) {
    //     printf("ht->buckets[%d].nItems = %d\n", i, ht->buckets[i].nItems);
    // }
    
    // printPageStats();
    
    free(ht->buckets);
    free(ht);
    free(rArray);

    return 0;
}