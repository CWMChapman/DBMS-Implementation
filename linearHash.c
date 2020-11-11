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

*/

void increment_next (hash_table* ht) {
    (*ht).level++;
    return;
}

ridPage* getBucketForInsert(hash_table* ht, int key) {

    return;
}

void insert(hash_table* ht) {
    


    return;
}

// Code to test the hash_table functions
int main(int argc, char** argv) {
    initPageManager();

    hash_table ht;
    ht.level = 0;
    ht.next = &ht.buckets[0];
    printf("%i\n", ht.level);
    increment_next(&ht);
    printf("%i\n", ht.level);









}