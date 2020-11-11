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

void treeInsert(pageptr tree, record toAdd) {
  // locate appropriate leaf
  // root non-full
  // HERE
  // if id not duplicate, add into leaf
  return;
}


// TESTING CODE
int main(int argc, char** argv) {
  initPageManager();

  printf("%lu\n", (PAGESIZE - sizeof(int)) / sizeof(record));
  printf("%lu\n", sizeof(record));

  rid rids[6];
  
  for (int i = 0; i < 6; ++i) {
    record r = { i, "Gregory", "Alice" };
    rids[i] = addRecord(r);
  }
  
  printRecordPage(pm->curRecordPage);

  printf("done\n");
  
  /*
  pageptr test = initTreeNode();
  printTreeNode(test);
  pageptr test2 = initTreeNode();
  test2.ptr.node->children[1].k = 5;
  test.ptr.node->children[0].p = test2.ptr.node;
  test.ptr.node->children[1].k = 8;
  test.ptr.node->nItems = 2;
  printTreeNode(test);
  */
  return 0;
}
