#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include<stdlib.h>
#include<stdio.h>

/* * * *
This is a stubby page manager. It is meant to simply log page I/Os, so for 
it to work properly  you need to go through it EVERY TIME YOU GET A NEW
PAGE! It is set up to be fairly straightforward: you simply instantiate a 
pageManager struct pointed to by <pm>, call getPage(<pageptr>) every 
time you want to read a page, and putPage(<pageptr>) every time you
want to write a page. These functions simply update some counters and return 
the pointer you gave them, so your code will work just fine without the page 
manager and it is on you to call it!
* * * */

// = = = =
// DATA TYPES, CONSTANTS
// = = = =
// bytes per page--MUST BE MULTIPLE OF 8
// sizes of structs--only works for 64-bit systems
// all of these constants are checked against sizeof() at the initialization
// of the page manager.
#define PAGESIZE 264
#define PAGEUNION_SIZE 8
#define PAGEPTR_SIZE 16
#define KP_SIZE 16
#define RECORD_SIZE 64
#define RID_SIZE 16
// number of items per page
#define RIDPAGE_ITEMS ((PAGESIZE - (2 * PAGEPTR_SIZE) - 8) / RID_SIZE)
// must be odd--round down by 1 if even
#define TREEPAGE_ITEMS (((PAGESIZE - 8) / KP_SIZE) - (((PAGESIZE - 8) / KP_SIZE + 1) % 2))
#define RECORDPAGE_ITEMS ((PAGESIZE - 8) / RECORD_SIZE)

// "INNER" STRUCTS--THINGS IN PAGES
typedef union {
  struct ridPage* rid;
  struct treePage* node;
  struct recordPage* rec;
} pageUnion;
// SIZE: 8 bytes


// struct to keep track of page pointers and their type
// type codes:
// 0 = ridPage
// 1 = treePage
// 2 = recordPage
typedef struct {
  int32_t type; // + 32 bit pad
  pageUnion ptr;
} pageptr;
// SIZE: 16 bytes

// union for treePage to elegantly interlace keys and pointers
typedef union kp {
  int32_t k;
  pageptr p;
} kp;
// SIZE: 16 bytes

// struct for passing values up a treeInsert
typedef struct passUp {
  pageptr ptr;
  int32_t key;
} passUp;
// SIZE: 24 bytes

// individual record. Arbitrarily set to 64 bytes.
typedef struct {
  int32_t id;
  char f1[30];
  char f2[30];
} record;
// SIZE: 64 bytes

// rid: points to a record page
typedef struct {
  int32_t id;
  int32_t slot;
  struct recordPage* page;
} rid;
// SIZE: 16 bytes

// PAGE STRUCTS
// ridPage: one leaf page or hash bucket. stores rids.
typedef struct ridPage {
  pageptr prev;
  pageptr next;
  int64_t nItems;
  rid rids[RIDPAGE_ITEMS];
} ridPage;

// treePage: one non-leaf node in the tree
typedef struct treePage {
  int64_t nItems;
  union kp children[TREEPAGE_ITEMS];
} treePage;

// recordPage: one actual page in file of records
typedef struct recordPage {
  int32_t nItems;
  int32_t emptySlots;
  record records[RECORDPAGE_ITEMS];
} recordPage;

// page manager: right now only tracks reads/writes.
typedef struct {
  int reads;
  int writes;
  recordPage* curRecordPage;
} pageManager;

// = = = =
// FUNCTION DECLARATIONS
// = = = =
// CALL ME FIRST THING YOU DO OR ELSE YOU WILL SEGFAULT
void initPageManager();

// function stubs to read and write from "disk"--in reality just update
// the counters
pageptr getPage(pageptr toGet);
void putPage(pageptr toPut);

// utilities to make a pageptr
pageptr genTreePageptr(treePage* ptr);
pageptr genRidPageptr(ridPage* rid);
pageptr genRecordPageptr(recordPage* rec);

// make a page
recordPage* initRecordPage();
ridPage* initRidPage();

// add a record
rid addRecord(record toAdd);
void remRecord(rid toRem);

// prints
void printRidPage(pageptr n);
void printTreePage(pageptr n);
void printRecordPage(pageptr n);
void printPage(pageptr n);

void printRid(rid r);
void printRecord(record r);

void printSizes();
void printPageStats();
void clearPageManager();

//extern pageManager* pm;
pageManager* pm;

#endif
