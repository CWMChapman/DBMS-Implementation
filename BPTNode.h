#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

#include "pageManager.h"

// unneeded--merge with PAGESIZE
#define NODESIZE 5

// DATA TYPES
typedef struct {
  int nItems; // TODO
  union kp* arr;
} BPTNode;

typedef union kp {
  int k;
  BPTNode* p;
} kp;
