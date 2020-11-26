#include "recVec.h"

recVec initRecVec() {
  record* newArr = malloc(4 * sizeof(record));
  return (recVec) { .nItems = 0, .size = 4, .arr = newArr };
}

void recVecPush(recVec* rv, record toPush) {
  if (rv->nItems == rv->size) rv->arr = realloc(rv->arr, rv->size *= 2);
  rv->arr[rv->nItems++] = toPush;
  return;
}

/*
record recVecPeek(recVec* rv) {
  return rv->arr[rv->nItems-1];
}

void recVecPop(recVec* rv) {
  if (--rv->nItems <= rv->size / 2) rv->arr = realloc(rv->arr, rv->size /= 2);
  return;
}
*/

void printRecVec(recVec rv) {
  printf("===============================\n");
  printf("size: %i\nnItems: %i\n", rv.size, rv.nItems);
  printf("id \t  f1 \t\t  f2\n");
  for (int i = 0; i < rv.nItems; ++i) {
    printf("%i \t| %s \t| %s\n", rv.arr[i].id, rv.arr[i].f1, rv.arr[i].f2);
  }
  printf("===============================\n");
  return;
}
