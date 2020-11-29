#ifndef TREE_H
#define TREE_H

#include "treeHelper.h" // brings in pageManager, string.h
#include "recVec.h"

pageptr initTree();
pageptr initTreeNode();
void treeInsert(pageptr* tree, record toAdd);
record treeSearch(pageptr tree, int id);
recVec treeRangeSearch(pageptr tree, int min, int max);

#endif
