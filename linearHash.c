#include "linearHash.h"



/*

FUNCTIONS:

    See rough algorithm in the textbook on page 381

    hash (returns bucket to insert into)
    insert (returns record)
    increment_next (void)
    delete (void)
    search (look_up) (returns record)
    split





    So im gonna need the index, which consists of ridPages

*/

int hash(int level, int key) {
    printf("level: %i\n", level);
    printf("before key: %i\n", key);
    for (int i = 31; i >= level; --i) {
        key &= ~(1UL << i); // this sets each of the bits we dont care about at this level to zero, so we can return an int that will represent the index into the array of buckets!
    }
    printf("after key: %i\n", key);
    // buf[buf_index] |= message[i] << (7-bit_index);

    return key;
}

void insert(hashTable* ht, record toAdd) {
    int key = toAdd.id;
    // int* hash_array = hash(key);
    int index = hash(ht->level, key);
    int ridPageIndex = ht->buckets[index].nItems;
    
    ht->buckets[index].rids[ridPageIndex] = addRecord(toAdd);

    return;
}


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
