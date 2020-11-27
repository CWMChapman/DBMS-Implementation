#include "pageManager.h"

pageManager* pm = NULL;

// checks sizes of all structs and pages
// prints an error and aborts if any are wrong
void checkSizes() {
  printf("CHECKING SIZES...\n");
  if (PAGESIZE % 8) {
    printf("ERROR: pageSize IS INCORRECT. ABORTING.\n(expected multiple of 8, got %i)\n",
           PAGESIZE);
    abort();
  }
  if (PAGESIZE < 120) {
    printf("ERROR: pageSize IS TOO SMALL. ABORTING.\n(must be at least 120, got %i)\n",
           PAGESIZE);
    abort();
  }
  if (sizeof(pageUnion) != PAGEUNION_SIZE) {
    printf("ERROR: pageUnion IS INCORRECT SIZE. ABORTING.\n(expected %i, got %lu)\n",
           PAGEUNION_SIZE, sizeof(pageUnion));
    abort();
  }
  if (sizeof(pageptr) != PAGEPTR_SIZE) {
    printf("ERROR: pageptr IS INCORRECT SIZE. ABORTING.\n(expected %i, got %lu)\n",
           PAGEPTR_SIZE, sizeof(pageptr));
    abort();
  }
  if (sizeof(kp) != KP_SIZE) {
    printf("ERROR: kp IS INCORRECT SIZE. ABORTING.\n(expected %i, got %lu)\n",
           KP_SIZE, sizeof(kp));
    abort();
  }
  if (sizeof(record) != RECORD_SIZE) {
    printf("ERROR: record IS INCORRECT SIZE. ABORTING.\n(expected %i, got %lu)\n",
           RECORD_SIZE, sizeof(record));
    abort();
  }
  if (sizeof(rid) != RID_SIZE) {
    printf("ERROR: rid IS INCORRECT SIZE. ABORTING.\n(expected %i, got %lu)\n",
           RID_SIZE, sizeof(rid));
    abort();
  }
  if (sizeof(ridPage) > PAGESIZE) {
    printf("ERROR: ridPage IS TOO BIG. ABORTING.\n(expected %i, got %lu)\n",
           PAGESIZE, sizeof(ridPage));
    abort();
  }
  if (sizeof(treePage) > PAGESIZE) {
    printf("ERROR: treePage IS TOO BIG. ABORTING.\n(expected %i, got %lu)\n",
           PAGESIZE, sizeof(treePage));
    abort();
  }
  if (sizeof(recordPage) > PAGESIZE) {
    printf("ERROR: recordPage IS TOO BIG. ABORTING.\n(expected %i, got %lu)\n",
           PAGESIZE, sizeof(recordPage));
    abort();
  }
  if (sizeof(ridPage) < PAGESIZE) printf(
    "INFO: ridPage IS POTENTIALLY UNDERSIZED.\n(is %lu, can be %i)\n",
    sizeof(ridPage), PAGESIZE);
  if (sizeof(treePage) < PAGESIZE) printf(
    "INFO: treePage IS POTENTIALLY UNDERSIZED.\n(is %lu, can be %i)\n",
    sizeof(treePage), PAGESIZE);
  if (sizeof(recordPage) < PAGESIZE) printf(
    "INFO: recordPage IS POTENTIALLY UNDERSIZED.\n(is %lu, can be %i)\n",
    sizeof(recordPage), PAGESIZE);
  return;
}

void initPageManager() {
  checkSizes();
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

rid addRecord(record toAdd) {
  // current page is full, time to add a new one
  if ((pm->curRecordPage->nItems) >= RECORDPAGE_ITEMS) {
    putPage(genRecordPageptr(pm->curRecordPage));
    pm->curRecordPage = initRecordPage();
  }
  // create rid
  rid ret = { toAdd.id, pm->curRecordPage->nItems, pm->curRecordPage };
  pm->curRecordPage->records[pm->curRecordPage->nItems] = toAdd;
  (pm->curRecordPage->nItems)++;
  return ret;
}

// removes one record. TODO: make more elegant
void remRecord(rid toRem) {
  toRem.page->records[toRem.slot].id = -1;
  if (++(toRem.page->emptySlots) == RECORDPAGE_ITEMS) free(toRem.page);
  printRecordPage(genRecordPageptr(toRem.page));
  return;
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
  ret->emptySlots = 0;
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
  printf("=====================================\nRID PAGE\naddress: %p\nnItems: %lli\n",
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
  printf("address: %p\nnItems: %lli\n", tp, tp->nItems);
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
  printf("=====================================\nRECORD PAGE\n");
  printf("address: %p\nnItems: %i\nemptySlots: %i\n", tp, tp->nItems, tp->emptySlots);
  for (int i = 0; i < tp->nItems; ++i) {
    printf("%i \t| %s \t| %s\n",
           tp->records[i].id,
           tp->records[i].f1,
           tp->records[i].f2);
  }
  printf("=====================================\n\n");
  return;
}

void printPage(pageptr n) {
  switch(n.type) {
  case 0:
    printRidPage(n);
    break;
  case 1:
    printTreePage(n);
    break;
  case 2:
    printRecordPage(n);
    break;
  }
}

void printRid(rid r) {
  printf("====================\nid: %i\npage: %p\nslot: %i\n====================\n",
         r.id, r.page, r.slot);
}

void printRecord(record r) {
  printf("=====================================\n");
  printf("%i\t| %s\t| %s\n", r.id, r.f1, r.f2);
  printf("=====================================\n\n");
  return;
}

void printSizes() {
  printf("==========================\nPAGE SIZES\n");
  printf("pagesize \t%i bytes\n"
         "treepage \t%i kps\n"
         "ridpage \t%i rids\n"
         "recordpage \t%i records\n",
         PAGESIZE, TREEPAGE_ITEMS, RIDPAGE_ITEMS, RECORDPAGE_ITEMS);
  /* printf("PAGE SIZES\nridPage \t%lu\ntreePage \t%lu\nrecordPage \t%lu\n\n", */
  /*        sizeof(ridPage), sizeof(treePage), sizeof(recordPage)); */
  /* printf("INTERNAL STRUCT SIZES\npageptr \t%lu\nkp \t\t%lu\nrecord \t\t%lu\nrid \t\t%lu\n", */
  /*        sizeof(pageptr), sizeof(kp), sizeof(record), sizeof(rid)); */
  printf("==========================\n");
  return;
}

void printPageStats() {
  printf("READS: %i\nWRITES: %i\n", pm->reads, pm->writes);
  return;
}
