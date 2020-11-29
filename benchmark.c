#include "tree.h"
#include "linearHash.h"
#include<math.h>

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

record* genSkewedRecords(int nRecords) {
  printf("generating skewed records...\n");
  record* ret = malloc(nRecords * sizeof(record));
  float nWaves = 2;
  int intensity = 100;
  float tau = 6.2831853 * nWaves;
  int c = 0;
  int x, jump;
  for (int i = 0; i < nRecords; ++i) {
    ret[i] = (record) { .id = c, .f1 = "Gregory", .f2 = "Alice" };
    x = tau * ((double) i / (double)nRecords);
    jump = (cos(x) + 1) * intensity;
    c += (jump == 0 ? 1 : jump);
  }
  return ret;
}

record* genRandomSkewedRecords(int nRecords) {
  printf("generating random skewed records...\n");
  record* ret = malloc(nRecords * sizeof(record));
  float nWaves = 2;
  int intensity = 100;
  float tau = 6.2831853 * nWaves;
  int c = 0;
  int x, jump;
  for (int i = 0; i < nRecords; ++i) {
    ret[i] = (record) { .id = c, .f1 = "Gregory", .f2 = "Alice" };
    x = tau * ((double) i / (double)nRecords);
    jump = (cos(x) + 1) * intensity;
    c += (jump == 0 ? 1 : jump);
  }

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

void benchmarkTreeRangeSearch(record* testArr, int nRecords, FILE* fout) {
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

  printf("tree range searching...\n");

  clearPageManager();
  treeRangeSearch(root, -1, -1);
  fprintf(fout, "RS-ALL \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);
  return;
}

void benchmarkHashRangeSearch(record* testArr, int nRecords, FILE* fout) {
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
  hashRangeSearch(ht, 0, nRecords-1);
  fprintf(fout, "ALL \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);
  return;
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

  freeTree(root);
  return;
}

void benchmarkHash(record* testArr, int nRecords, FILE* fout) {
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

  free(random);

  fputs("\n========\nIN ORDER RECORDS\n", fout);
  record* inOrder = genInOrderRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(inOrder, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(inOrder, nRecords, fout);

  free(inOrder);

  fputs("\n========\nREVERSE ORDER RECORDS\n", fout);
  record* reverse = genReverseOrderRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(reverse, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(reverse, nRecords, fout);

  free(reverse);

  fputs("\n========\nSKEWED RECORDS\n", fout);
  record* skew = genSkewedRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(skew, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(skew, nRecords, fout);

  free(skew);

  fputs("\n========\nRANDOM SKEWED RECORDS\n", fout);
  record* randSkew = genRandomSkewedRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(randSkew, nRecords, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(randSkew, nRecords, fout);

  free(randSkew);
  
  fclose(fout);

  //for python plots

  // fout = fopen("plotStats.txt", "w+");
  // writeSizes(fout);

  // record* random10 = genRandomRecords(10);
  // record* random100 = genRandomRecords(100);
  // record* random1000 = genRandomRecords(1000);
  // record* random10000 = genRandomRecords(10000);
  // record* random100000 = genRandomRecords(100000);
  // record* random1000000 = genRandomRecords(1000000);
  // record* random10000000 = genRandomRecords(10000000);
  // printf("done generating record arrays\n");


  // fputs("\nTREE\n", fout);
  // benchmarkTreeRangeSearch(random10, 10, fout);
  // benchmarkTreeRangeSearch(random100, 100, fout);
  // benchmarkTreeRangeSearch(random1000, 1000, fout);
  // benchmarkTreeRangeSearch(random10000, 10000, fout);
  // benchmarkTreeRangeSearch(random100000, 100000, fout);
  // benchmarkTreeRangeSearch(random1000000, 1000000, fout);
  // benchmarkTreeRangeSearch(random10000000, 10000000, fout);

  // fputs("\n========\nHASH\n", fout);
  // benchmarkHashRangeSearch(random10, 10, fout);
  // benchmarkHashRangeSearch(random100, 100, fout);
  // benchmarkHashRangeSearch(random1000, 1000, fout);
  // benchmarkHashRangeSearch(random10000, 10000, fout);
  // benchmarkHashRangeSearch(random100000, 100000, fout);
  // benchmarkHashRangeSearch(random1000000, 1000000, fout);
  // benchmarkHashRangeSearch(random10000000, 10000000, fout);

  // fclose(fout);


  return 0;

}
