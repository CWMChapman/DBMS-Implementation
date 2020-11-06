#include "pageManager.h"

<<<<<<< HEAD
static pageManager* pm;
=======
pageManager* pm = NULL;

//static pageManager* pm;
>>>>>>> ddbd2951b3d00776bdea4b6ba73f8d1433c88313

void initPageManager() {
  pm = malloc(sizeof(pageManager));
  pm->reads = 0;
  pm->writes = 0;
}

<<<<<<< HEAD
page* getPage(page* toGet) {
=======
pageptr getPage(pageptr toGet) {
>>>>>>> ddbd2951b3d00776bdea4b6ba73f8d1433c88313
  pm->reads++;
  return toGet;
}

<<<<<<< HEAD
void putPage(page* toPut) {
=======
void putPage(pageptr toPut) {
>>>>>>> ddbd2951b3d00776bdea4b6ba73f8d1433c88313
  pm->writes++;
  return;
}

<<<<<<< HEAD
// what do hash table pages look like?
=======
>>>>>>> ddbd2951b3d00776bdea4b6ba73f8d1433c88313

int main() {
  initPageManager();
  printf("reads: %i, writes: %i\n", pm->reads, pm->writes);
  return 0;
}
