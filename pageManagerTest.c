#include "pageManager.h"

int main() {
  initPageManager();
  printf("reads: %i, writes: %i\n", pm->reads, pm->writes);
  printf("page size: %i\nrid page size: %li\ntree page size: %li\nrecord page size: %li\n",
         PAGESIZE, sizeof(ridPage), sizeof(treePage), sizeof(recordPage));
  return 0;
}
