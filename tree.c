#include "tree.h"

// BASIC UTILITY FUNCTIONS
// initializer
// NOTE: returns NULL on error. All calls should be guarded accordingly.
pageptr initTreeNode() {
  treePage* ret = malloc(sizeof(treePage));
  for (int i = 0; i < TSIZE; ++i) {
    if (i % 2) ret->children[i].p.ptr.node = NULL;
    else ret->children[i].k = -1;
  }
  return genTreePageptr(ret);
}

// returns root of tree
pageptr initTree() {
  return initTreeNode();
}

void insert(pageptr n, record toAdd, pageptr* newChild) {
  if (n.type == 1) { // n tree node (i.e. non-leaf)
    // search for appropriate location
    // TODO: binary search instead of linear
    treePage* t = n.ptr.node;
    pageptr next = t->children[0].p;
    for (int i = 1; i < t->nItems; i += 2) {
      if (t->children[i].k <= toAdd.id) {
        next = t->children[i+1].p;
      }
    }
    next = getPage(next);
    //printRidPage(next);
    insert(next, toAdd, newChild);
    // handle newChild if needed
  }
  else if (n.type == 0) { // leaf node
    ridPage* r = n.ptr.rid;
    if (r->nItems == RSIZE) {
      ridPage* newLeaf = initRidPage();
      pageptr newPageptr = getPage(genRidPageptr(newLeaf));
      // update linked list pointers
      newLeaf->next = r->next;
      newLeaf->prev = newPageptr;
      r->next = newPageptr;
      // send median up
      // LEFT OFF HERE
    }
    else {
      // find location in leaf node
      // TODO: binary search instead of linear
      int idx = r->nItems;
      for (int i = r->nItems - 1; i >=0; --i) {
        if (r->rids[i].id > toAdd.id) idx = i;
      }
      // make space
      memmove(&(r->rids[idx+1]), &(r->rids[idx]), sizeof(rid) * (r->nItems - idx));
      r->rids[idx] = addRecord(toAdd);
      (r->nItems)++;
    }
  }
  return;
}

void treeInsert(pageptr tree, record toAdd) {
  // tree is empty
  if (tree.ptr.node->nItems == 0) {
    // initialize a new leaf node w/ rid pointing to the newly added record
    pageptr newLeaf = getPage(genRidPageptr(initRidPage()));
    rid newRid = addRecord(toAdd);
    ridPage* tmp = newLeaf.ptr.rid;
    tmp->rids[0] = newRid;
    (tmp->nItems)++;
    // add leaf node to tree
    tree.ptr.node->children[0].p = newLeaf;
    tree.ptr.node->nItems = 1;
    return;
  }
  pageptr* dummy = malloc(sizeof(pageptr));
  *dummy = genTreePageptr(NULL);
  insert(tree, toAdd, dummy);
  return;
}


// TESTING CODE
int main(int argc, char** argv) {
  initPageManager();

  printf("%lu\n", (PAGESIZE - sizeof(int)) / sizeof(record));

  pageptr root = initTreeNode();
  record r0 = { 5, "Gregory", "Alice" };
  record r1 = { 7, "Grigori", "Alicia" };
  record r2 = { 3, "Grogoro", "Alamo" };
  record r3 = { 9, "Gruggug", "Albania" };
  record r4 = { 6, "Google", "Amazon" };

  treeInsert(root, r0);
  treeInsert(root, r1);
  treeInsert(root, r2);
  treeInsert(root, r3);
  treeInsert(root, r4);

  printTreePage(root);

  printRidPage(genRidPageptr(root.ptr.node->children[0].p.ptr.rid));
  printRecordPage(genRecordPageptr(root.ptr.node->children[0].p.ptr.rid->rids[0].page));
  
  /*
  pageptr test = initTreeNode();
  printTreeNode(test);
  pageptr test2 = initTreeNode();
  test2.ptr.node->children[1].k = 5;
  test.ptr.node->children[0].p = genTreePageptr(test2.ptr.node);
  test.ptr.node->children[1].k = 8;
  test.ptr.node->nItems = 2;
  printTreeNode(test);
  */
  return 0;
}
