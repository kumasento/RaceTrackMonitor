
#include "CacheGenerator.h"

#define MOD(x, y) ((x) % (y) == 0)
#define ERROR(str) { fprint(stderr, (str)); return NULL; }

// cacheAddr [ Tag | Set | Bias ]

Cache* Generator(int addrSpaceSize, 
                 int cacheSize, 
                 int cacheBlockSize, 
                 int cacheBlockNum,
                 int cacheWayNum) {

    int numBlocks   = cacheSize / cacheBlockSize;
    int numLines    = numBlocks / cacheBlockNum;
    int numSets     = numLines / cacheWayNum;
    
    //if (MOD(cacheSize, cacheBlockSize)) ERROR("MOD(cacheSize,cacheBlockSize)");
    //if (MOD(numBlocks, cacheBlockNum))  ERROR("MOD(numBlocks, cacheBlockNum)");
    //if (MOD(numLines, cacheWayNum))     ERROR("MOD(numLines, cacheWayNum)");

     
}
