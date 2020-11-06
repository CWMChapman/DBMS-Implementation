#include<stdlib.h>
#include<stdio.h>

/* * * *
This is a stubby page manager. It is meant to simply log page I/Os, so for 
it to work properly  you need to go through it EVERY TIME YOU GET A NEW
PAGE! It is set up to be fairly straightforward: you simply instantiate a 
pageManager struct pointed to by <pm>, call getPage(<pm>, <pointer>) every 
time you want to read a page, and putPage(<pm>, <pointer>) every time you
want to write a page. These functions simply update some counters and return 
the pointer you gave them, so your code will work just fine without the page 
manager and it is on you to call it!
* * * */

// = = = =
// DATA TYPES, CONSTANTS
// = = = =
// bytes per page
#define PAGESIZE 256

// "INNER" STRUCTS--THINGS IN PAGES
// union for treePage to elegantly interlace keys and pointers
typedef union kp{
  int k;
  struct treePage* p;
} kp;

// individual record. Arbitrarily set to 64 bytes.
typedef struct {
  int id;
  char f1[30];
  char f2[30];
} record;

// rid: points to a record page
typedef struct {
  record* page;
  int slot;
} rid;

// PAGE STRUCTS
// ridPage: one leaf page or hash bucket. stores rids.
typedef struct {
  struct ridPage* prev;
  struct ridPage* next;
  rid rids[(PAGESIZE - 2 * sizeof(struct ridPage*)) / sizeof(rid)];
} ridPage;

// treePage: one non-leaf node in the tree
typedef struct {
  int nItems;
  union kp children[(PAGESIZE - sizeof(int)) / sizeof(union kp)];
} treePage;

// recordPage: one actual record
typedef struct {
  record records[PAGESIZE / sizeof(record)];
} recordPage;

typedef union {
  ridPage* rid;
  treePage* node;
  recordPage* rec;
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

// page manager: right now only tracks reads/writes.
typedef struct {
  int reads;
  int writes;
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

extern pageManager* pm;
