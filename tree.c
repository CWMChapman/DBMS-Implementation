#include "tree.h"

// BASIC UTILITY FUNCTIONS
// initializer
// NOTE: returns NULL on error. All calls should be guarded accordingly.
pageptr initTreeNode() {
  treePage* ret = malloc(sizeof(treePage));
  for (int i = 0; i < TSIZE; ++i) {
    if (i % 2) ret->children[i].p = NULL;
    else ret->children[i].k = -1;
  }
  return genTreePageptr(ret);
}

pageptr initLeafNode() {
  ridPage* ret = malloc(sizeof(ridPage));
  ret->prev = NULL;
  ret->next = NULL;
  return genRidPageptr(ret);
}

// returns root of tree
pageptr initTree() {
  return initTreeNode();
}

// print
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

void addRecord(pageptr tree, record toAdd) {
  // locate appropriate leaf
  // root non-full
  
  // if id not duplicate, add into leaf
  return;
}


// TESTING CODE
int main(int argc, char** argv) {
  initPageManager();
  pageptr test = initTreeNode();
  printTreeNode(test);
  pageptr test2 = initTreeNode();
  test2.ptr.node->children[1].k = 5;
  test.ptr.node->children[0].p = test2.ptr.node;
  test.ptr.node->children[1].k = 8;
  test.ptr.node->nItems = 2;
  printTreeNode(test);
/*
  ridPage* newRid = malloc(sizeof(ridPage));
  pageptr ridptr = genRidPageptr(newRid);
  printf("%i\n", ridptr.type);
  */
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
