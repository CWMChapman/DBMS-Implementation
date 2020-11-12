#include<string.h>
#include<time.h>

#include "pageManager.h"

pageptr initTree();
void printTreePage(pageptr n);
void treeInsert(pageptr* tree, record toAdd);
