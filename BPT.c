#include "BPT.h"
/*
// BASIC UTILITY FUNCTIONS
// initializer
// NOTE: returns NULL on error. All calls should be guarded accordingly.
pageptr initBPTNode() {
  treePage* ret = malloc(sizeof(treePage));
  for (int i = 0; i < NODESIZE; i += 2) {
    ret->children[i].p = NULL;
  }
  return ret;
}

// print
void printBPTNode(pageptr n) {
  for (int i = 0; i < NODESIZE; ++i) {
    if (i % 2) printf("K: %i ", n->arr[i].k);
    else printf("P: %i ", n->arr[i].p == NULL ? 0 : n->arr[i].p->arr[1].k);
  }
  printf("\n");
  return;
}
*/

// TESTING CODE
int main(int argc, char** argv) {
  initPageManager();

  // test root node
  /*
  BPTNode* test = initBPTNode();
  printBPTNode(test);
  // test child of root
  BPTNode* test2 = initBPTNode();
  test2->arr[1].k = 5;
  test->arr[0].p = test2;
  printBPTNode(test);

  printf("%i\n", pm->reads);
  page* testPage = malloc(sizeof(page));
  getPage(testPage);
  printf("%i\n", pm->reads);
  */
  return 0;
}
