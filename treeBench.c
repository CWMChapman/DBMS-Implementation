#include "tree.h"

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

  /*
    printf("before range search\n");
    printPageStats();
    treeRangeSearch(*root, -1, -1);
    printf("after range search\n");
    printPageStats();
  */
  
  /*
    rid rem1 = addRecord(randRecords[0]);
    rid rem2 = addRecord(randRecords[1]);
    rid rem3 = addRecord(randRecords[2]);
    rid rem4 = addRecord(randRecords[3]);
    printRecordPage(genRecordPageptr(pm->curRecordPage));
    remRecord(rem1);
    remRecord(rem2);
    remRecord(rem3);
    remRecord(rem4);
  */
  
  if (checkTree(*root, nRecords)) exploreTree(*root);

  // IMPORTANT ASSUMPTIONS
  // 1. generating new pages does not cost a read, only a write
  printPageStats();

}
