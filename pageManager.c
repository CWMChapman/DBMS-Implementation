#include "pageManager.h"

pageManager* pm = NULL;

void initPageManager() {
  pm = malloc(sizeof(pageManager));
  pm->reads = 0;
  pm->writes = 0;
  pm->curRecordPage = initRecordPage();
}

pageptr getPage(pageptr toGet) {
  pm->reads++;
  return toGet;
}

void putPage(pageptr toPut) {
  pm->writes++;
  return;
}

// TODO: deletion, handling non-full pages
rid addRecord(record toAdd) {
  // current page is full, time to add a new one
  if ((pm->curRecordPage->nItems) >= (PAGESIZE - sizeof(int)) / sizeof(record)) {
    pm->curRecordPage = initRecordPage();
    (pm->writes)++; // writing the current record page out (???)
  }
  // create rid
  rid ret = { toAdd.id, pm->curRecordPage, pm->curRecordPage->nItems };
  pm->curRecordPage->records[pm->curRecordPage->nItems] = toAdd;
  (pm->curRecordPage->nItems)++;
  return ret;
}

pageptr genTreePageptr(treePage* ptr) {
  pageUnion retUnion;
  retUnion.node = ptr;
  pageptr ret = {1, retUnion};
  return ret;
}

pageptr genRidPageptr(ridPage* rid) {
  pageUnion retUnion;
  retUnion.rid = rid;
  pageptr ret = {0, retUnion};
  return ret;
}

pageptr genRecordPageptr(recordPage* rec) {
  pageUnion retUnion;
  retUnion.rec = rec;
  pageptr ret = {2, retUnion};
  return ret;
}

recordPage* initRecordPage() {
  recordPage* ret = malloc(sizeof(recordPage));
  ret->nItems = 0;
  return ret;
}

ridPage* initRidPage() {
  ridPage* ret = malloc(sizeof(ridPage));
  ret->next = genRidPageptr(NULL);
  ret->prev = genRidPageptr(NULL);
  ret->nItems = 0;
  return ret;
}

// prints
void printRidPage(pageptr n) {
  if (n.type != 0) {
    printf("ERROR: wrong page type (expected 0, got %i)\n", n.type);
  }
  ridPage* tp = n.ptr.rid;
  printf("=====================================\nRID PAGE\naddress: %p\nnItems: %i\n",
         tp, tp->nItems);
  printf("prev: %p\nnext: %p\n", tp->prev.ptr.rid, tp->next.ptr.rid);
  for (int i = 0; i < tp->nItems; ++i) {
    printf("%i \t| %p \t| %i\n",
           tp->rids[i].id,
           tp->rids[i].page,
           tp->rids[i].slot);
  }
  printf("=====================================\n\n");
  return;
}

void printTreePage(pageptr n) {
  if (n.type != 1) {
    printf("ERROR: wrong page type (expected 1, got %i)\n", n.type);
    return;
  }
  treePage* tp = n.ptr.node;
  printf("=====================================\nTREE PAGE\n");
  printf("address: %p\nnItems: %i\n", tp, tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    if (i % 2) printf("K: %i ", tp->children[i].k);
    else printf("P: %p (%i)", tp->children[i].p.ptr.node, tp->children[i].p.type);
    printf("\n");
  }
  printf("=====================================\n\n");
  return;
}

void printRecordPage(pageptr n) {
  if (n.type != 2) {
    printf("ERROR: wrong page type (expected 2, got %i)\n", n.type);
  }
  recordPage* tp = n.ptr.rec;
  printf("=====================================\nRECORD PAGE\naddress: %p\nnItems: %i\n", tp, tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    printf("%i \t| %s \t| %s\n",
           tp->records[i].id,
           tp->records[i].f1,
           tp->records[i].f2);
  }
  printf("=====================================\n\n");
  return;
}

void printSizes() {
  printf("======================\n");
  printf("DEFINED SIZES\npagesize \t%i\ntsize \t\t%lu\nrsize \t\t%lu\n\n",
         PAGESIZE, TSIZE, RSIZE);
  printf("PAGE SIZES\nridPage \t%lu\ntreePage \t%lu\nrecordPage \t%lu\n\n",
         sizeof(ridPage), sizeof(treePage), sizeof(recordPage));
  printf("INTERNAL STRUCT SIZES\npageptr \t%lu\nkp \t\t%lu\nrecord \t\t%lu\nrid \t\t%lu\n",
         sizeof(pageptr), sizeof(kp), sizeof(record), sizeof(rid));
  printf("======================\n");
  return;
}
