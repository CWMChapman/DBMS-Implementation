#include "pageManager.h"

int main(int argc, char** argv) {
  treePage* testPage = malloc(sizeof(treePage));
  pageptr test = genTreePageptr(testPage);
  passUp* p = malloc(sizeof(passUp));
  *p = (passUp) { test, 5 };
  printf("size: %lu\n", sizeof(recordPage));
  return 0;
}
