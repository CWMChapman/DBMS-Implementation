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
// no. of records per page
#define PAGESIZE_RECORDS 10
// no. of bytes per page
#define PAGESIZE_BYTES 16 + 64 * PAGESIZE_RECORDS

// this is pretty arbitrary--set up to be 64 bytes
typedef struct {
  int id;
  char f1[30];
  char f2[30];
} record;

// single page in the file of records
typedef struct {
  // note: for the linear hash, only the next pointer is needed. prev
  // is there simply so we can reuse the page struct.
  struct page* next;
  struct page* prev;
  record records[PAGESIZE_RECORDS];
} page;

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
page* getPage(page* toGet);
void putPage(page* toPut);



extern pageManager* pm;
//static pageManager* pm;
