#include "tree.h"

// BASIC UTILITY FUNCTIONS
// initializer
// NOTE: returns NULL on error. All calls should be guarded accordingly.
// TODO: move to pageManager
pageptr initTreeNode() {
  treePage* ret = malloc(sizeof(treePage));
  for (int i = 0; i < TSIZE; ++i) {
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
    // printf("id: %i, idx: %i\n", toAdd.id, idx);
    next = getPage(next);
    insert(next, toAdd, newChild);

    // newChild not null--need to add it
    if (newChild->key != -1) {
      int newKey = newChild->key; // keep these values so we can overwrite newChild
      pageptr newPtr = newChild->ptr;

       // full--need to split first
      if (t->nItems >= TSIZE) {
        treePage* newPage = malloc(sizeof(treePage));
        int median = TSIZE / 2 - ((TSIZE / 2 + 1) % 2);
        t->nItems = median;
        newPage->nItems = median;
        
        memmove(&(newPage->children[0]), &(t->children[median+1]), median * sizeof(kp));
        *newChild = (passUp) { t->children[median].k, genTreePageptr(newPage) };
        
        if (toAdd.id >= t->children[median].k) t = newPage;
      }
      // not full--newChild should be back to null
      else newChild->key = -1;
      
      // insert into t
      // TODO: binary search instead of linear
      int idx = t->nItems;
      for (int i = t->nItems - 2; i > 0; i -= 2) {
        //printf("KEY: %i\n", t->children[i].k);
        if (t->children[i].k > newKey) idx = i;
      }
      // make space
      memmove(&(t->children[idx+2]), &(t->children[idx]), sizeof(kp) * (t->nItems - idx));
      t->children[idx].k = newKey;
      t->children[idx+1].p = newPtr;
      t->nItems += 2;
    }
  }
  
  // leaf node--inserting at this level
  else if (n.type == 0) {
    ridPage* r = n.ptr.rid; // convenience
    
    // need to split the node
    if (r->nItems == RSIZE) {
      ridPage* newLeaf = initRidPage();
      pageptr newPageptr = getPage(genRidPageptr(newLeaf));
      
      // update linked list pointers
      newLeaf->next = r->next;
      newLeaf->prev = newPageptr;
      r->next = newPageptr;
      
      // split values--median and all subsequent values move to newLeaf
      int median = RSIZE / 2;
      newChild->key = r->rids[median].id;
      newChild->ptr = genRidPageptr(newLeaf);
      memmove(&(newLeaf->rids), &(r->rids[median]), sizeof(rid) * (RSIZE - median));
      newLeaf->nItems = RSIZE - median;
      r->nItems = median;
      
      // set r to correct node--only needs to change if in new node
      if (toAdd.id >= r->rids[median].id) {
        r = newLeaf;
      }
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
  }
  return;
}

void treeInsert(pageptr* tree, record toAdd) {
  // tree is empty
  if (tree->ptr.node->nItems == 0) {
    // initialize a new leaf node w/ rid pointing to the newly added record
    pageptr newLeaf = getPage(genRidPageptr(initRidPage()));
    rid newRid = addRecord(toAdd);
    ridPage* tmp = newLeaf.ptr.rid;
    tmp->rids[0] = newRid;
    (tmp->nItems)++;
    // add leaf node to tree
    tree->ptr.node->children[0].p = newLeaf;
    tree->ptr.node->nItems = 1;
    return;
  }
  passUp* toPass = malloc(sizeof(passUp));
  toPass->key = -1; // ASSUME NO NEGATIVE KEYS
  insert(*tree, toAdd, toPass);
  // handle splitting root--make new root, add pointers to split old root
  if (toPass->key != -1) {
    pageptr newRoot = initTreeNode();
    // put the old root on the left
    newRoot.ptr.node->children[0].p = genTreePageptr(tree->ptr.node);
    // new key value
    newRoot.ptr.node->children[1].k = toPass->key;
    // new child on right
    newRoot.ptr.node->children[2].p = toPass->ptr;
    newRoot.ptr.node->nItems = 3;
    *tree = newRoot;
  }
  return;
}

record* genRandomRecords(int nRecords) {
  // generate in-order array
  record r;
  strcpy(r.f1, "Gregory");
  strcpy(r.f2, "Alice");
  record* ret = malloc(nRecords * sizeof(record));
  for (int i = 0; i < nRecords; ++i) {
    r.id = i;
    ret[i] = r;
  }
  // shuffle array--swap each spot with a different one
  /*
  record tmp;
  int randInt;
  srand(time(NULL));
  for (int i = 0; i < nRecords; ++i) {
    tmp = ret[i];
    randInt = rand() % nRecords;
    ret[i] = ret[randInt];
    ret[randInt] = tmp;
  }
  */
  return ret;
}

void printTree(pageptr root) {
  while (root.type != 0) root = root.ptr.node->children[0].p;
  while (root.ptr.rid->next.ptr.rid != NULL) {
    printRidPage(root);
    root = root.ptr.rid->next;
  }
}

// TESTING CODE
int main(int argc, char** argv) {
  initPageManager();

  printf("%lu\n", RSIZE);

  pageptr* root = malloc(sizeof(pageptr));
  *root = initTreeNode();
  
  int nRecords = 150;
  record* randRecords = genRandomRecords(nRecords);
  for (int i = 0; i < nRecords; i += 2) {
    record r = randRecords[i];
    // printf("inserting %i\n", r.id);
    treeInsert(root, r);
  }

  // printf("TREE:\n");
  // printTree(*root);
  // STOPS AT 128--WHY???
  
  for (int i = 1; i < nRecords; i += 2) {
    record r = randRecords[i];
    // printf("inserting %i\n", r.id);
    treeInsert(root, r);
  }

  // printf("TREE:\n");
  // printTree(*root);

  // TODO: WHY DOES ROOT NOT CHANGE? SHOULD WRITE SEARCH TO CHECK

  printSizes();
  
  return 0;
}
