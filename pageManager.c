#include "pageManager.h"

pageManager* pm = NULL;

void initPageManager() {
  pm = malloc(sizeof(pageManager));
  pm->reads = 0;
  pm->writes = 0;
  pm->curRecordPage = initRecordPage();
}

recordPage* initRecordPage() {
  recordPage* ret = malloc(sizeof(recordPage));
  ret->nItems = 0;
}

// TODO: deletion, handling non-full pages
void addRecord(record toAdd) {
  // current page is full, time to add a new one
  if ((pm->curRecordPage->nItems) >= (PAGESIZE - sizeof(int)) / sizeof(record)) {
    pm->curRecoredPage = initRecordPage();
    (pm->writes)++; // writing the current record page out (???)
  }
  pm->curRecordPage[pm->curRecordPage->nItems] = toAdd;
  (pm->curRecordPage->nItems)++;
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
