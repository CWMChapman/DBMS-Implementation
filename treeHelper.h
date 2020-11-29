#ifndef TREEHELPER_H
#define TREEHELPER_H

#include <string.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include "pageManager.h"

// tree info
void printRids(pageptr root);
void exploreTree(pageptr root);

// validity
int checkTree(pageptr root, int nItems);

// generate records
record* genRandomRecords(int nRecords);

#endif
