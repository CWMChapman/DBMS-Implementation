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
  printf("========\nRID PAGE\nnItems: %i\n", tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    printf("%i | %p | %i\n",
           tp->rids[i].id,
           tp->rids[i].page,
           tp->rids[i].slot);
  }
  printf("========\n\n");
  return;
}

void printTreePage(pageptr n) {
  if (n.type != 1) {
    printf("ERROR: wrong page type (expected 1, got %i)\n", n.type);
  }
  treePage* tp = n.ptr.node;
  printf("========\nTREE PAGE\n");
  printf("nItems: %i\n| ", tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    if (i % 2) printf("K: %i ", tp->children[i].k);
    else printf("P: %i ",
                tp->children[i].p.ptr.node == NULL ? 0 : tp->children[i].p.ptr.node->children[1].k);
    printf("| ");
  }
  printf("\n========\n\n");
  return;
}

void printRecordPage(pageptr n) {
  if (n.type != 2) {
    printf("ERROR: wrong page type (expected 2, got %i)\n", n.type);
  }
  recordPage* tp = n.ptr.rec;
  printf("========\nRECORD PAGE\nnItems: %i\n", tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    printf("%i | %s | %s\n",
           tp->records[i].id,
           tp->records[i].f1,
           tp->records[i].f2);
  }
  printf("========\n\n");
  return;
}
