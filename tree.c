#include "tree.h"

// BASIC UTILITY FUNCTIONS
// initializer
// NOTE: returns NULL on error. All calls should be guarded accordingly.
// TODO: move to pageManager
pageptr initTreeNode() {
  treePage* ret = malloc(sizeof(treePage));
  for (int i = 0; i < TREEPAGE_ITEMS; ++i) {
    if (i % 2) ret->children[i].p.ptr.node = NULL;
    else ret->children[i].k = -1;
  }
  ret->nItems = 0;
  return genTreePageptr(ret);
}

// returns root of tree
pageptr initTree() {
  return initTreeNode();
}

void insert(pageptr n, record toAdd, passUp* newChild) {
  // n tree node (i.e. non-leaf)
  if (n.type == 1) { 
    // find appropriate location, insert into it
    // TODO: binary search instead of linear
    treePage* t = n.ptr.node;
    pageptr next = t->children[0].p;
    int idx = 0;
    for (int i = 1; i < t->nItems; i += 2) {
      if (t->children[i].k <= toAdd.id) {
        next = t->children[i+1].p;
        idx = i;
      }
    }
    // read next page into memory, insert on it
    next = getPage(next);
    insert(next, toAdd, newChild);

    // newChild not null--need to add it
    if (newChild->key != -1) {
      int newKey = newChild->key; // keep these values so we can overwrite newChild
      pageptr newPtr = newChild->ptr;

      // full--need to split first
      if (t->nItems >= TREEPAGE_ITEMS) {
        treePage* newPage = malloc(sizeof(treePage));
        int median = TREEPAGE_ITEMS / 2 - ((TREEPAGE_ITEMS / 2 + 1) % 2);
        *newChild = (passUp) { .ptr = genTreePageptr(newPage), .key = t->children[median].k };
        t->nItems = median;
        newPage->nItems = (TREEPAGE_ITEMS - median - 1);
        memmove(&(newPage->children[0]),
                &(t->children[median+1]),
                (TREEPAGE_ITEMS - median - 1) * sizeof(kp));
        
        // write current page, insert into new page
        if (toAdd.id >= t->children[median].k) {
          t = newPage;
          putPage(n);
        }
        // write new page, continue insert on current page
        else putPage(genTreePageptr(newPage));
      }
      // not full--newChild should be back to null
      else newChild->key = -1;
      
      // insert into t
      // TODO: binary search instead of linear
      int idx = t->nItems;
      for (int i = t->nItems - 2; i > 0; i -= 2) if (t->children[i].k > newKey) idx = i;
      // make space, add items
      memmove(&(t->children[idx+2]), &(t->children[idx]), sizeof(kp) * (t->nItems - idx));
      t->children[idx].k = newKey;
      t->children[idx+1].p = newPtr;
      t->nItems += 2;

      // done inserting, write results back to memory
      putPage(genTreePageptr(t));
    }
  }
  
  // leaf node--inserting at this level
  else if (n.type == 0) {
    ridPage* r = n.ptr.rid; // convenience
    // need to split the node
    if (r->nItems == RIDPAGE_ITEMS) {
      ridPage* newLeaf = initRidPage();
      pageptr newPageptr = genRidPageptr(newLeaf);
      
      // update linked list pointers
      newLeaf->next = r->next;
      newLeaf->prev = n;
      r->next = newPageptr;
      
      // split values--median and all subsequent values move to newLeaf
      int median = RIDPAGE_ITEMS / 2;
      newChild->key = r->rids[median].id;
      newChild->ptr = newPageptr; // genRidPageptr(newLeaf);
      memmove(&(newLeaf->rids), &(r->rids[median]), sizeof(rid) * (RIDPAGE_ITEMS - median));
      newLeaf->nItems = RIDPAGE_ITEMS - median;
      r->nItems = median;
      
      // set r to correct node, write other node
      if (toAdd.id >= r->rids[median].id) {
        r = newLeaf;
        putPage(n);
      }
      else putPage(newPageptr);
    }
    // insert into node
    // TODO: binary search instead of linear
    int idx = r->nItems;
    for (int i = r->nItems - 1; i >=0; --i) {
      if (r->rids[i].id > toAdd.id) idx = i;
    }
    
    // make space in node
    memmove(&(r->rids[idx+1]), &(r->rids[idx]), sizeof(rid) * (r->nItems - idx));
    r->rids[idx] = addRecord(toAdd);
    (r->nItems)++;
    
    putPage(genRidPageptr(r)); // done inserting, write node back to disk
  }
  return;
}

void treeInsert(pageptr* tree, record toAdd) {
  // tree is empty
  if (tree->ptr.node->nItems == 0) {
    // initialize a new leaf node w/ rid pointing to the newly added record
    pageptr newLeaf = genRidPageptr(initRidPage());
    rid newRid = addRecord(toAdd);
    ridPage* tmp = newLeaf.ptr.rid;
    tmp->rids[0] = newRid;
    (tmp->nItems)++;
    // add leaf node to tree
    tree->ptr.node->children[0].p = newLeaf;
    tree->ptr.node->nItems = 1;
    putPage(newLeaf);
    return;
  }

  passUp* toPass = malloc(sizeof(passUp));
  toPass->key = -1; // ASSUME NO NEGATIVE KEYS
  insert(*tree, toAdd, toPass);
  // handle splitting root--make new root, add pointers to split old root
  if (toPass->key != -1) {
    pageptr newRoot = initTreeNode();
    newRoot.ptr.node->children[0].p = genTreePageptr(tree->ptr.node);
    newRoot.ptr.node->children[1].k = toPass->key;
    newRoot.ptr.node->children[2].p = toPass->ptr;
    newRoot.ptr.node->nItems = 3;
    *tree = newRoot;
  }
  return;
}

record treeSearch(pageptr tree, int id) {
  pageptr cur = getPage(tree);
  int i;
  while (cur.type == 1) {
    // TODO: binary search
    // find appropriate child
    i = 0;
    while (i < cur.ptr.node->nItems - 1 &&
           cur.ptr.node->children[i+1].k <= id) i += 2;
    cur = getPage(cur.ptr.node->children[i].p);
  }
  i = 0;
  while (i < cur.ptr.rid->nItems && cur.ptr.rid->rids[i].id < id) ++i;
  // THIS SHOULD BE A PAGE READ BUT NOT RELEVANT FOR COMPARISON TO HASH
  return (cur.ptr.rid->rids[i].id == id) ?
    cur.ptr.rid->rids[i].page->records[cur.ptr.rid->rids[i].slot] :
    (record) { .id = -1, .f1 = "NO DATA", .f2 = "NO DATA" };
}

recVec treeRangeSearch(pageptr tree, int min, int max) {
  if (max == -1) max = INT_MAX;
  pageptr cur = getPage(tree);
  int i;
  // find first id
  while (cur.type == 1) {
    i = 0;
    while (i < cur.ptr.node->nItems - 1 &&
           cur.ptr.node->children[i+1].k <= min) i += 2;
    cur = getPage(cur.ptr.node->children[i].p);
  }
  i = 0;
  while (i < cur.ptr.rid->nItems && cur.ptr.rid->rids[i].id < min) ++i;
  recVec ret = initRecVec();
  record curRecord = cur.ptr.rid->rids[i].page->records[cur.ptr.rid->rids[i].slot];
  while (curRecord.id <= max) {
    recVecPush(&ret, curRecord);
    if (i == cur.ptr.rid->nItems - 1) {
      cur = getPage(cur.ptr.rid->next);
      i = 0;
      if (cur.ptr.rid == NULL) break;
    }
    else ++i;
    curRecord = cur.ptr.rid->rids[i].page->records[cur.ptr.rid->rids[i].slot];
  }
  return ret;
}

/*
// TESTING CODE
int main(int argc, char** argv) {
  initPageManager();

  // printSizes();

  pageptr* root = malloc(sizeof(pageptr));
  *root = initTreeNode();
  int nRecords = 150;
  if (argc == 2) {
    nRecords = atoi(argv[1]);
  }
  record* randRecords = genRandomRecords(nRecords);
  
  for (int i = 0; i < nRecords; ++i) {
    record r = randRecords[i];
    treeInsert(root, r);
  }

  printRecord(treeSearch(*root, 5));
  
  // printRecVec(treeRangeSearch(*root, -1, -1));

  
  printf("before range search\n");
  printPageStats();
  treeRangeSearch(*root, -1, -1);
  printf("after range search\n");
  printPageStats();
  
  
  
  rid rem1 = addRecord(randRecords[0]);
  rid rem2 = addRecord(randRecords[1]);
  rid rem3 = addRecord(randRecords[2]);
  rid rem4 = addRecord(randRecords[3]);
  printRecordPage(genRecordPageptr(pm->curRecordPage));
  remRecord(rem1);
  remRecord(rem2);
  remRecord(rem3);
  remRecord(rem4);
  
  if (checkTree(*root, nRecords)) exploreTree(*root);

  // IMPORTANT ASSUMPTIONS
  // 1. generating new pages does not cost a read, only a write
  printPageStats();
  
  return 0;
}
*/
