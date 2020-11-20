#include "treeHelper.h"

void printRids(pageptr root) {
  while (root.type != 0) root = root.ptr.node->children[0].p;
  while (root.ptr.rid != NULL) {
    printRidPage(root);
    root = root.ptr.rid->next;
  }
  return;
}

int checkTreeNodes(pageptr cur, int min, int max) {
  if (cur.type != 1) return 0;
  int n = cur.ptr.node->nItems;
  if (cur.ptr.node->children[1].k < min ||
      cur.ptr.node->children[n-2].k >= max) {
    printf("DISCREPANCY DETECTED\nMIN: %i\nMAX: %i\n", min, max);
    printTreePage(cur);
    printf("\n");
    return 1;
  }
  
  int ret = checkTreeNodes(cur.ptr.node->children[0].p,
                           min,
                           cur.ptr.node->children[1].k);
  for (int i = 3; i < n - 2; i += 2) {
    if (cur.ptr.node->children[i].k <= cur.ptr.node->children[i-2].k) {
      printf("DISCREPANCY DETECTED\nMIN: %i\nMAX: %i\n", min, max);
      printTreePage(cur);
      printf("\n");
      return 1;
    }
    ret |= checkTreeNodes(cur.ptr.node->children[i-1].p,
                          cur.ptr.node->children[i-2].k,
                          cur.ptr.node->children[i].k);
  }
  return ret | checkTreeNodes(cur.ptr.node->children[n-1].p,
                              cur.ptr.node->children[n-2].k,
                              max);
}

pageptr getMin(pageptr root) {
  pageptr cur = root;
  while (cur.type != 0) cur = cur.ptr.node->children[0].p;
  return cur;
}

int checkLeaves(pageptr root, int nItems) {
  int curValue = 0;
  pageptr curRidPage = getMin(root);
  int i = 0;
  while (curValue < nItems) {
    if (i == curRidPage.ptr.rid->nItems) {
      i = 0;
      curRidPage = curRidPage.ptr.rid->next;
    }
    if (curRidPage.ptr.rid->rids[i].id != curValue) {
      printf("LEAF DISCREPANCY DETECTED\n");
      printf("EXPECTED %i, GOT %i\n", curValue, curRidPage.ptr.rid->rids[i].id);
      printRidPage(curRidPage);
      return 1;
    }
    ++i;
    ++curValue;
  }
  return 0;
}

// checks tree for validity
int checkTree(pageptr root, int nItems) {
  if (root.type == 0) return 0; // at leaf node; no work to do
  printf("CHECKING TREE...\n");
  if (checkTreeNodes(root, 0, INT_MAX)) {
    printf("TREE DISCREPANCY DETECTED\n");
    return 1;
  }
  printf("TREE TESTS PASSED.\nCHECKING LEAVES...\n");
  if (checkLeaves(root, nItems)) {
    printf("LEAF DISCREPANCY DETECTED\n");
    return 1;
  }
  printf("LEAF TESTS PASSED.\n");
  return 0;
}

// THIS IS SUPER EASY TO BREAK, IT'S ONLY MEANT FOR TESTING
void exploreTree(pageptr root) {
  char command = 'n';
  int pos = 0;
  ridPage* ptr;
  while (command != 'q') {
    printf("CURRENT PAGE:\n");
    printTreePage(root);
    printf("enter command (char int): ");
    fflush(stdin);
    scanf("%c %i", &command, &pos);
    // g: go to; p: print; q: up
    switch(command) {
    case 'a':
      printRids(root);
      break;
    case 'g':
      if (pos < root.ptr.node->nItems && root.ptr.node->children[pos].p.type == 1) {
        exploreTree(root.ptr.node->children[pos].p);
      }
      break;
    case 'x':
      printf("ptr: ");
      scanf("%p", &ptr);
      printRidPage(genRidPageptr(ptr));
      break;
    case 'p':
      if (pos < root.ptr.node->nItems) {
        if (root.ptr.node->children[pos].p.type == 0) {
          printRidPage(root.ptr.node->children[pos].p);
        }
        else {
          printTreePage(root.ptr.node->children[pos].p);
        }
      }
      break;
    case 'q':
      return;
    }
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
  
  record tmp;
  int randInt;
  srand(time(NULL));
  for (int i = 0; i < nRecords; ++i) {
    tmp = ret[i];
    randInt = rand() % nRecords;
    ret[i] = ret[randInt];
    ret[randInt] = tmp;
  }
  
  return ret;
}
