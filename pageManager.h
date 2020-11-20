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
#define PAGESIZE 280
#define TSIZE ((PAGESIZE - sizeof(int)) / sizeof(union kp) -            \
               ((PAGESIZE - sizeof(int)) / sizeof(union kp) + 1) % 2)
#define RSIZE (PAGESIZE - 2 * sizeof(struct ridPage*) - sizeof(int)) / sizeof(rid)
#define RPP ((PAGESIZE - (2 * sizeof(int))) / sizeof(record))

// "INNER" STRUCTS--THINGS IN PAGES
typedef union {
  struct ridPage* rid;
  struct treePage* node;
  struct recordPage* rec;
} pageUnion;


// struct to keep track of page pointers and their type
// type codes:
// 0 = ridPage
// 1 = treePage
// 2 = recordPage
typedef struct {
  int type;
  pageUnion ptr;
} pageptr;

// union for treePage to elegantly interlace keys and pointers
typedef union kp {
  int k;
  pageptr p;
} kp;

// struct for passing values up a treeInsert
typedef struct passUp {
  int key;
  pageptr ptr;
} passUp;

// individual record. Arbitrarily set to 64 bytes.
typedef struct {
  int id;
  char f1[30];
  char f2[30];
} record;

// rid: points to a record page
typedef struct {
  int id;
  struct recordPage* page;
  int slot;
} rid;

// PAGE STRUCTS
// ridPage: one leaf page or hash bucket. stores rids.
typedef struct ridPage {
  //struct ridPage* prev;
  //struct ridPage* next;
  pageptr prev;
  pageptr next;
  int nItems;
  rid rids[(PAGESIZE - 2 * sizeof(pageptr) - sizeof(int)) / sizeof(rid)];
} ridPage;

// treePage: one non-leaf node in the tree
typedef struct treePage {
  int nItems;
  union kp children[(PAGESIZE - sizeof(int)) / sizeof(union kp)];
} treePage;

// recordPage: one actual page in file of records
typedef struct recordPage {
  int nItems;
  int emptySlots;
  record records[RPP];
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

void printRid(rid r);

void printSizes();
void printPageStats();

extern pageManager* pm;
