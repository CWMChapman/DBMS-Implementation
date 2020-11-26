#include "pageManager.h"

// Simple dynamic array. Commented functions implemented, but not needed.

typedef struct recVec {
  int nItems;
  int size;
  record* arr;
} recVec;
  
recVec initRecVec();

void recVecPush(recVec* rv, record toPush);

//record recVecPeek(recVec* rv);

// void recVecPop(recVec* rv);

void printRecVec(recVec rv);
