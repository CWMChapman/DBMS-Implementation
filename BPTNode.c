#include "BPTNode.h"

// BASIC UTILITY FUNCTIONS
// initializer
// NOTE: returns NULL on error. All calls should be guarded accordingly.
BPTNode* initBPTNode() {
  if (NODESIZE % 2 == 0) {
    printf("ERROR: invalid size (should be odd)\n");
    return NULL;
  }
  BPTNode* ret = malloc(sizeof(BPTNode));
  ret->arr = malloc(NODESIZE * sizeof(kp));
  for (int i = 0; i < NODESIZE; i += 2) {
    ret->arr[i].p = NULL;
  }
  return ret;
}

// print
void printBPTNode(BPTNode* n) {
  for (int i = 0; i < NODESIZE; ++i) {
    if (i % 2) printf("K: %i ", n->arr[i].k);
    else printf("P: %i ", n->arr[i].p == NULL ? 0 : n->arr[i].p->arr[1].k);
  }
  printf("\n");
  return;
}

void benchmark() {
  int nItems = 1000000000; // 1 billion (= ~8GB)
  int* array = malloc(nItems * sizeof(int));
  for (int i = 0; i < nItems - 1; ++i) {
    array[i] = i;
  }
  
  // time constructs
  struct timespec start;
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  // "simple" approach--move each one by one
  // 2.603s (avg of 5 runs)
  /*
  for (int i = nItems - 1; i > 0; --i) {
    array[i] = array[i-1];
  }
  */

  // memmove approach
  // memcpy possibly problematic, but a tiny bit faster
  // .457s (avg of 6, some outliers)
  memmove(array + 1, array, (nItems - 1) * sizeof(int));

  // compute time
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed = (end.tv_sec - start.tv_sec) +
    (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
  
  printf("completed in %f s\n", elapsed);
}

// TESTING CODE
int main(int argc, char** argv) {
  benchmark();
  return 0;
  // test root node
  BPTNode* test = initBPTNode();
  printBPTNode(test);
  // test child of root
  BPTNode* test2 = initBPTNode();
  test2->arr[1].k = 5;
  test->arr[0].p = test2;
  printBPTNode(test);
  return 0;
}
