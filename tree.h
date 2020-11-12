#include<string.h>
#include<time.h>

#include "pageManager.h"

pageptr initTree();
void printTreeNode(pageptr n);
void treeInsert(pageptr tree, record toAdd);
