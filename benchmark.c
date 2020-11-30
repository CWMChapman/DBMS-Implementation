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

void shuffleRecords(record* records, int nRecords) {
  printf("generating random skewed records...\n");
  
  record tmp;
  int randInt;
  srand(time(NULL));
  for (int i = 0; i < nRecords; ++i) {
    tmp = records[i];
    randInt = rand() % nRecords;
    records[i] = records[randInt];
    records[randInt] = tmp;
  }
  return;
}

void writeStatsForPlot() {
  record* random10 = genRandomSkewedRecords(10);
  record* random100 = genRandomSkewedRecords(100);
  record* random1000 = genRandomSkewedRecords(1000);
  record* random10000 = genRandomSkewedRecords(10000);
  record* random100000 = genRandomSkewedRecords(100000);
  record* random1000000 = genRandomSkewedRecords(1000000);
  // record* random10000000 = genRandomRecords(10000000);
  // record* random100000000 = genRandomRecords(100000000); //comp cant handle
  printf("done generating record arrays\n");

  FILE* fout = fopen("plotRangeSearchStats.txt", "w+");
  writeSizes(fout);
  
  fputs("nRecords nReads\n", fout);
  fputs("\nTREE\n", fout);
  benchmarkTreeRangeSearch(random10, 10, fout);
  benchmarkTreeRangeSearch(random100, 100, fout);
  benchmarkTreeRangeSearch(random1000, 1000, fout);
  benchmarkTreeRangeSearch(random10000, 10000, fout);
  benchmarkTreeRangeSearch(random100000, 100000, fout);
  benchmarkTreeRangeSearch(random1000000, 1000000, fout);

  fputs("\nHASH\n", fout);
  benchmarkHashRangeSearch(random10, 10, fout);
  benchmarkHashRangeSearch(random100, 100, fout);
  benchmarkHashRangeSearch(random1000, 1000, fout);
  benchmarkHashRangeSearch(random10000, 10000, fout);
  benchmarkHashRangeSearch(random100000, 100000, fout);
  benchmarkHashRangeSearch(random1000000, 1000000, fout);
  
  fputs("\n", fout);
  fclose(fout);

  fout = fopen("plotSearchStats.txt", "w+");
  writeSizes(fout);

  fputs("nRecords nReads\n", fout);
  fputs("\nTREE\n", fout);
  benchmarkTreeSearch(random10, 10, fout);
  benchmarkTreeSearch(random100, 100, fout);
  benchmarkTreeSearch(random1000, 1000, fout);
  benchmarkTreeSearch(random10000, 10000, fout);
  benchmarkTreeSearch(random100000, 100000, fout);
  benchmarkTreeSearch(random1000000, 1000000, fout);

  fputs("\nHASH\n", fout);
  benchmarkHashSearch(random10, 10, fout);
  benchmarkHashSearch(random100, 100, fout);
  benchmarkHashSearch(random1000, 1000, fout);
  benchmarkHashSearch(random10000, 10000, fout);
  benchmarkHashSearch(random100000, 100000, fout);
  benchmarkHashSearch(random1000000, 1000000, fout);
  
  fputs("\n", fout);
  fclose(fout);
}


void benchmarkTreeRangeSearch(record* testArr, int nRecords, FILE* fout) {
  pageptr root = initTree();
  fprintf(fout, "%i ", nRecords);

  printf("tree inserting...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) treeInsert(&root, testArr[i]);

  printf("tree range searching...\n");

  clearPageManager();
  treeRangeSearch(root, -1, -1);
  fprintf(fout, "%i\n", pm->reads);
  return;
}

void benchmarkHashRangeSearch(record* testArr, int nRecords, FILE* fout) {
  hashTable* ht = initHashTable();
  fprintf(fout, "%i ", nRecords);

  printf("hash inserting...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) hashInsert(ht, testArr[i], -1);

  printf("hash range searching...\n");

  clearPageManager();
  hashRangeSearch(ht, 0, nRecords-1);
  fprintf(fout, "%i\n", pm->reads);
  return;
}

void benchmarkTreeSearch(record* testArr, int nRecords, FILE* fout) {
  pageptr root = initTree();
  fprintf(fout, "%i ", nRecords);

  printf("tree inserting...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) treeInsert(&root, testArr[i]);

  printf("tree searching...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) treeSearch(root, testArr[i].id);
  fprintf(fout, "%i\n", pm->reads);
  
  return;
}

void benchmarkHashSearch(record* testArr, int nRecords, FILE* fout) {
  hashTable* ht = initHashTable();
  fprintf(fout, "%i ", nRecords);

  printf("hash inserting...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) hashInsert(ht, testArr[i], -1);

  printf("hash searching...\n");
  
  clearPageManager();
  for (int i = 0; i < nRecords; ++i) hashSearch(ht, testArr[i].id);
  fprintf(fout, "%i\n", pm->reads);

  printf("hash range searching...\n");
  
  return;
}


void benchmarkTree(record* testArr, int nRecords, int* keys, FILE* fout) {
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
  treeRangeSearch(root, -1, keys[0]);
  fprintf(fout, "FIRST 3 \t| R: %i \tW: %i\n", pm->reads, pm->writes);
  
  clearPageManager();
  treeRangeSearch(root, keys[6], -1);
  fprintf(fout, "LAST 3 \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, -1, keys[1]);
  fprintf(fout, "FIRST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, keys[5], -1);
  fprintf(fout, "LAST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, -1, keys[2]);
  fprintf(fout, "FIRST Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, keys[2], keys[3]);
  fprintf(fout, "SECOND Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  treeRangeSearch(root, keys[4], -1);
  fprintf(fout, "LAST Q \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);
  
  clearPageManager();
  treeRangeSearch(root, -1, -1);
  fprintf(fout, "ALL \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  freeTree(root);
  return;
}

void benchmarkHash(record* testArr, int nRecords, int* keys, FILE* fout) {
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
  hashRangeSearch(ht, 0, keys[0]);
  fprintf(fout, "FIRST 3 \t| R: %i \tW: %i\n", pm->reads, pm->writes);
  
  clearPageManager();
  hashRangeSearch(ht, keys[6], keys[7]);
  fprintf(fout, "LAST 3 \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, 0, keys[1]);
  fprintf(fout, "FIRST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, keys[5], keys[7]);
  fprintf(fout, "LAST 10 \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, 0, keys[2]);
  fprintf(fout, "FIRST Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, keys[2], keys[3]);
  fprintf(fout, "SECOND Q \t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, keys[4], keys[7]);
  fprintf(fout, "LAST Q \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);

  clearPageManager();
  hashRangeSearch(ht, 0, keys[7]);
  fprintf(fout, "ALL \t\t| R: %i \tW: %i\n", pm->reads, pm->writes);
  return;
}

int main(int argc, char** argv) {
  int nRecords = 150;
  if (argc == 2) nRecords = atoi(argv[1]);

  // third, tenth, first quartile end, second q end,
  // third q end, tenth from end, third from end, last
  int keys[8] = {2, 9, nRecords / 4, nRecords / 2, (3 * nRecords) / 4, nRecords - 11, nRecords-4, nRecords - 1};
  
  initPageManager();
  printSizes();

  FILE* fout;
  fout = fopen("stats.txt", "w+");

  writeSizes(fout);
  
  fputs("RANDOM RECORDS\n", fout);
  record* random = genRandomRecords(nRecords);
  
  fputs("TREE\n", fout);
  benchmarkTree(random, nRecords, keys, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(random, nRecords, keys, fout);

  free(random);

  fputs("\n========\nIN ORDER RECORDS\n", fout);
  record* inOrder = genInOrderRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(inOrder, nRecords, keys, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(inOrder, nRecords, keys, fout);

  free(inOrder);

  fputs("\n========\nREVERSE ORDER RECORDS\n", fout);
  record* reverse = genReverseOrderRecords(nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(reverse, nRecords, keys, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(reverse, nRecords, keys, fout);

  free(reverse);

  fputs("\n========\nSKEWED RECORDS\n", fout);
  record* skew = genSkewedRecords(nRecords);
  for (int i = 0; i < 8; ++i) keys[i] = skew[keys[i]].id;
  if (keys[7] >= INT_MAX) {
    printf("ERROR: max value too large. Try reducing intensity of skew.\n");
    abort();
  }
  // for (int i = 0; i < 8; ++i) printf("%i\n", keys[i]);

  fputs("TREE\n", fout);
  benchmarkTree(skew, nRecords, keys, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(skew, nRecords, keys, fout);

  fputs("\n========\nRANDOM SKEWED RECORDS\n", fout);
  // record* randSkew = genRandomSkewedRecords(nRecords);
  shuffleRecords(skew, nRecords);

  fputs("TREE\n", fout);
  benchmarkTree(skew, nRecords, keys, fout);
  
  fputs("\nHASH\n", fout);
  benchmarkHash(skew, nRecords, keys, fout);

  free(skew);
  
  fclose(fout);






  // for python plots 
  // writeStatsForPlot();



  return 0;

}
