#include "linearHash.h"

/*
FUNCTIONS:

    See rough algorithm in the textbook on page 381

    hash
    insert
    lookUp
    delete
*/

int hash(int level, int key) {
    printf("level: %i\n", level);
    printf("before key: %i\n", key);
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    printf("after key: %i\n", key);

    return key;
}

void insert(hashTable* ht, record toAdd) {
    int key = toAdd.id;
    // int* hash_array = hash(key);
    int bucketIndex = hash(ht->level, key);
    int ridPageIndex = ht->buckets[bucketIndex].nItems;
    rid recordID = addRecord(toAdd);
    ht->buckets[bucketIndex].rids[ridPageIndex] = recordID;
    ht->buckets[bucketIndex].nItems++;
    putPage(genRecordPageptr(recordID.page));

    return;
}

// record lookup(hashTable* ht, int key) {
//     int bucketIndex = hash(ht->level, key);

//     for (int i = 0; i < ht->buckets[bucketIndex].nItems; ++i) {
//         if (ht->buckets[bucketIndex].rids[i].) // i need to look through the records from the record ids to search for the key and tehn return the whole record.
//     }

// }

hashTable* initHashTable() {
    hashTable* ht = malloc(sizeof(hashTable)); // why do i have to do this instead of hashTable* ht;
    ht->level = 2; // set hash level to 2 (that is, we are going to be looking at the 2 least significant bits in the binary represntation of the key -- see hashing scheme)
    ht->num_buckets = INITIAL_NUM_BUCKETS;
    for(int i = 0; i < ht->num_buckets; ++i) {
        ht->buckets[i] = *initRidPage();
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
    record r0 = {5, "zero", "zero"};
    insert(ht, r0);


    return 0;
}
