#include <string.h>
#include <unistd.h>
#include <time.h>
#include "pageManager.h"

// tree info
void printRids(pageptr root);
void exploreTree(pageptr root);

// generate records
record* genRandomRecords(int nRecords);
