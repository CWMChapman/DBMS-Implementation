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



*/






int hash(int level, int key) {
    level += 2;
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    // printf("key: %d\n", key);
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
    
    // increment level if next is pointing to the last bucket in that level also double buckets
    if (ht->next_index == getNumBucketsAtLevel(ht->level)-1) {
        doubleBuckets(ht);
        ht->level++;
    }

    if (ht->next_index == 0) {
        doubleBuckets(ht);
    }


    while (bucketToBeSplit.next.type != -1) {
        for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
            // I need to look through the records from the record ids to search for the key and then return the whole record.
            rid recordID = bucketToBeSplit.rids[i];
            record r = recordID.page->records[recordID.slot];
            insert(ht, r, ht->level+1);
        }
        bucketToBeSplit = *bucketToBeSplit.next.ptr.rid;
    }
    for (int i = 0; i < bucketToBeSplit.nItems; ++i) {
        rid recordID = bucketToBeSplit.rids[i];
        record r = recordID.page->records[recordID.slot];
        insert(ht, r, ht->level+1);
    }
    ht->next_index++;
    return;
}



void insert(hashTable* ht, record toAdd, int optionalLevel) {
    int key = toAdd.id;
    ridPage* bucket;
    if (optionalLevel == -1) {
        // use ht's level
        int bucketIndex = hash(ht->level, key);
        if (bucketIndex <= ht->next_index && ht->num_buckets > 4) {
            bucketIndex = hash(ht->level+1, key);
            printf("bucketIndex: %d\n", bucketIndex);
        }
        bucket = &ht->buckets[bucketIndex];
    }
    else {
        // use the argument-provided level
        int bucketIndex = hash(optionalLevel, key);
        bucket = &ht->buckets[bucketIndex];
    }

    // the bucket has an overflow bucket if the type is not equal to -1. -1 implies that there is no overflow.
    while (bucket->next.type != -1) {
        bucket = bucket->next.ptr.rid;
    }

    // Is the number of items == to the max size (capacity) of the bucket? if so, add overflow bucket
    //If you add an overflow bucket, call split so that it can split the bucket for the next pointer and increment the level
    if (bucket->nItems == sizeof(bucket->rids)/sizeof(bucket->rids[0])) {
        split(ht);
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


record lookup(hashTable* ht, int key, int optionalLevel) {
    ridPage* bucket;
    if (optionalLevel != -1) {
        // use ht's level
        int bucketIndex = hash(ht->level, key);
        bucket = &ht->buckets[bucketIndex];
    }
    else {
        // use the argument-provided level
        int bucketIndex = hash(optionalLevel, key);
        bucket = &ht->buckets[bucketIndex];
    }

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


randArray* initRandArray() {
    int size = 10000;
    randArray* rA = malloc(sizeof(randArray));
    rA->size = size;
    for (int i = 0; i < size; ++i)
        rA->array[i] = i;
    return rA;
}

int randInt(int upper_bound) {
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    int t_mil = _t.tv_sec*1000 + lround(_t.tv_nsec/1.0e6);
    srand(t_mil); //using current time in milliseconds as a seed to get the random number
    return rand() % upper_bound;
}

int getRandKey(randArray* rA) {
    int randIndex = randInt(rA->size);
    int key = rA->array[randIndex];
    rA->array[randIndex] = rA->array[rA->size];
    rA->size -= 1;
    return key;
}


// Code to test the hash_table functions
int main(int argc, char** argv) {
    initPageManager();
    hashTable* ht = initHashTable();
    randArray* rA = initRandArray();
    // record r0 = {5, "five", "5"};
    // insert(ht, r0, -1);


    
    int n = 1000;

    // insert n key-value pairs, with random keys
    // for (int i = 0; i < n; i++) {
    //     insert(ht, (record){.id = getRandKey(rA), .f1 = "i", .f2 = "empty"}, -1);
    //     // printf("inserting: %d\n", i);
    // }

    // insert n key-value pairs, sequentially, but this leads to bottom heavy hash table
    for (int i = 0; i < n; i++) {
        insert(ht, (record){.id = i, .f1 = "i", .f2 = "empty"}, -1);
    }


    //lookup n key-value pairs
    // for (int i = 0; i < n; i++) {
    //     record l = lookup(ht, i, -1);
    //     // printf("key: %d, value: %s\n", l.id, l.f1);
    // }




    printf("the number of buckets: %d\n", ht->num_buckets);
    for (int i = 0; i < ht->num_buckets; ++i) {
        printf("buckets[%d].nItems = %d\n", i, ht->buckets[i].nItems);
    }
    printf("next index: %d\n", ht->next_index);
    printf("getNumBucketsAtLevel(%d)-1 = %d", ht->level, getNumBucketsAtLevel(ht->level)-1);


    return 0;
}
