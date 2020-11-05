#include "pageManager.h"

pageManager* pm = NULL;

//static pageManager* pm;

void initPageManager() {
  pm = malloc(sizeof(pageManager));
  pm->reads = 0;
  pm->writes = 0;
}

page* getPage(page* toGet) {
  pm->reads++;
  return toGet;
}

void putPage(page* toPut) {
  pm->writes++;
  return;
}

// what do hash table pages look like?

/*
int main() {
  initPageManager();
  printf("reads: %i, writes: %i\n", pm->reads, pm->writes);
  return 0;
}
*/
