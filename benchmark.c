#include "tree.h"
#include "linearHash.h"

record* genInOrderRecords(int nRecords) {
  printf("generating in-order records...\n");
  record r;
  strcpy(r.f1, "Gregory");
  strcpy(r.f2, "Alice");
  record* ret = malloc(nRecords * sizeof(record));
  for (int i = 0; i < nRecords; ++i) {
    r.id = i;
    ret[i] = r;
  }
  return ret;
}

record* genReverseOrderRecords(int nRecords) {
  printf("generating reverse-order records...\n");
  record r;
  strcpy(r.f1, "Gregory");
  strcpy(r.f2, "Alice");
  record* ret = malloc(nRecords * sizeof(record));
  for (int i = nRecords - 1; i >= 0; --i) {
    r.id = i;
    ret[i] = r;
  }
  return ret;
}

record* genRandomRecords(int nRecords) {
  printf("generating random records...\n");
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

void benchmarkTree(record* testArr, int nRecords, FILE* fout) {
  pageptr root = initTree();
  fprintf(fout, "nRecords: %i\n", nRecords);

  printf("tree inserting...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) treeInsert(&root, testArr[i]);
  fprintf(fout, "INSERT \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  printf("tree searching...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) treeSearch(root, testArr[i].id);
  fprintf(fout, "SEARCH \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  fputs("RANGES\n", fout);

  printf("tree range searching...\n");

  clearPageManager();
  treeRangeSearch(root, -1, 3);
  fprintf(fout, "FIRST 3 \t| R: %i \tW: %i\n", pm->reads, pm->writes);
  
  clearPageManager();
  treeRangeSearch(root, nRecords - 4, -1);
  fprintf(fout, "LAST 3 \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, -1, 10);
  fprintf(fout, "FIRST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, nRecords - 11, -1);
  fprintf(fout, "LAST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, -1, nRecords / 4);
  fprintf(fout, "FIRST Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, nRecords / 4, nRecords / 2);
  fprintf(fout, "SECOND Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, (3 * nRecords) / 4, -1);
  fprintf(fout, "LAST Q \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, -1, -1);
  fprintf(fout, "ALL \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);
  return;
}

void benchmarkHash(record* testArr, int nRecords, FILE* fout) {
  // pageptr root = initTree();
  hashTable* ht = initHashTable();
  fprintf(fout, "nRecords: %i\n", nRecords);

  printf("hash inserting...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) hashInsert(ht, testArr[i], -1);
  fprintf(fout, "INSERT \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  printf("hash searching...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) hashSearch(ht, testArr[i].id);
  fprintf(fout, "SEARCH \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  fputs("RANGES\n", fout);

  printf("hash range searching...\n");

  clearPageManager();
  hashRangeSearch(ht, 0, 3);
  fprintf(fout, "FIRST 3 \t| R: %i \tW: %i\n", pm->reads, pm->writes);
  
  clearPageManager();
  hashRangeSearch(ht, nRecords - 4, nRecords-1);
  fprintf(fout, "LAST 3 \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, 0, 10);
  fprintf(fout, "FIRST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, nRecords - 11, nRecords-1);
  fprintf(fout, "LAST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, 0, nRecords / 4);
  fprintf(fout, "FIRST Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, nRecords / 4, nRecords / 2);
  fprintf(fout, "SECOND Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, (3 * nRecords) / 4, nRecords-1);
  fprintf(fout, "LAST Q \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, 0, nRecords-1);
  fprintf(fout, "ALL \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);
  return;
}

int main(int argc, char** argv) {
  int nRecords = 150;
  if (argc == 2) nRecords = atoi(argv[1]);
  
  initPageManager();
  printSizes();

  FILE* fout;
  fout = fopen("stats.txt", "w+");

  writeSizes(fout);
  
  fputs("RANDOM RECORDS\n", fout);
  record* random = genRandomRecords(nRecords);
  
  fputs("TREE\n", fout);
  benchmarkTree(random, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(random, nRecords, fout);

  fputs("\n========\nIN ORDER RECORDS\n", fout);
  record* inOrder = genInOrderRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(inOrder, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(inOrder, nRecords, fout);

  fputs("\n========\nREVERSE ORDER RECORDS\n", fout);
  record* reverse = genReverseOrderRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(reverse, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(reverse, nRecords, fout);
  
  fclose(fout);

  return 0;

}
