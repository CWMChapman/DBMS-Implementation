#include "treeHelper.h"

void printRids(pageptr root) {
  while (root.type != 0) root = root.ptr.node->children[0].p;
  while (root.ptr.rid != NULL) {
    printRidPage(root);
    root = root.ptr.rid->next;
  }
  return;
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
