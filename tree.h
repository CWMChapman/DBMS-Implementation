// #include<time.h>

#include "treeHelper.h" // brings in pageManager, string.h
#include "recVec.h"

pageptr initTree();
void treeInsert(pageptr* tree, record toAdd);
rid treeSearch(pageptr tree, int id);
