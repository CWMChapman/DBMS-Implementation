// #include<time.h>

#include "treeHelper.h" // brings in pageManager, string.h
#include "recVec.h"

pageptr initTree();
void treeInsert(pageptr* tree, record toAdd);
record treeSearch(pageptr tree, int id);
recVec treeRangeSearch(pageptr tree, int min, int max);
