#include "pageManager.h"

pageManager* pm = NULL;

void initPageManager() {
  pm = malloc(sizeof(pageManager));
  pm->reads = 0;
  pm->writes = 0;
}

pageptr getPage(pageptr toGet) {
  pm->reads++;
  return toGet;
}

void putPage(pageptr toPut) {
  pm->writes++;
  return;
}

int main() {
  initPageManager();
  printf("reads: %i, writes: %i\n", pm->reads, pm->writes);
  return 0;
}
