#include <cstdlib>

#include "Cache.h"

#define MASK(x, m) ((x) & (m))

int Cache::toCacheIdx(int sid, int lid, int bid) {
    return  bid + 
            lid * numBlock + 
            sid * numBlock * numLine;
}

// Please take care of the div ops
// Input the power of the size
Cache::Cache(int cacheSize,
             int blockSize,
             int numWay,
             int biasBitWidth) {

    int blocksPerLine = 1<<biasBitWidth;

    numBlock    = cacheSize / blockSize;
    numLine     = numBlock / blocksPerLine;
    numSet      = numLine / numWay;

    blocks   = (CacheBlock*) malloc(sizeof(CacheBlock)*numBlock);

    biasmask = (1<<biasBitWidth)-1;
    setmask  = (numSet-1) << biasBitWidth;
    tagmask  = ~(biasmask | setmask);
}

CacheUnit Cache::getBias(Addr addr) {
    return biasmask & addr;
}

CacheUnit Cache::getSet(Addr addr) {
    return setmask & addr;
}

CacheUnit Cache::getTag(Addr addr) {
    return tagmask & addr;
}

