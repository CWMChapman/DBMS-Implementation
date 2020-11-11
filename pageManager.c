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

recordPage* initRecordPage() {
  recordPage* ret = malloc(sizeof(recordPage));
  ret->nItems = 0;
  return ret;
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

// prints
// print treeNode
void printTreeNode(pageptr n) {
  treePage* tp = n.ptr.node;
  printf("type: %i\n", n.type);
  printf("nItems: %i\n", tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    if (i % 2) printf("K: %i ", tp->children[i].k);
    else printf("P: %i ", tp->children[i].p == NULL ? 0 : tp->children[i].p->children[1].k);
  }
  printf("\n");
  return;
}

void printRecordPage(recordPage* tp) {
  // recordPage* tp = n.ptr.rec;
  printf("========\nnItems: %i\n", tp->nItems);
  for (int i = 0; i < tp->nItems; ++i) {
    printf("%i | %s | %s\n",
           tp->records[i].id,
           tp->records[i].f1,
           tp->records[i].f2);
  }
  printf("========\n");
  return;
}
