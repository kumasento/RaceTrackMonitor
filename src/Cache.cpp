#include <iostream>
#include <cstdlib>

#include "Cache.h"

#define MASK(x, m) ((x) & (m))

using namespace std;

void Cache::toIdx(int* idx, int* sid, int* lid, int* bid) {
    if (*bid >= numBlock || *lid >= numLine || *sid >= numSet)
        *idx = -1;
    else
        *idx =  *bid + 
                *lid * numBlock + 
                *sid * numBlock * numLine;
}
void Cache::toDim3(int* idx, int *sid, int *lid, int *bid) {
    if (*idx == -1 || *idx > numBlock * numLine * numSet)
        *sid = *lid = *bid = -1;
    else {
        *sid = *idx / (numBlock * numLine);
        *lid = *idx % (numBlock * numLine);
        *bid = *lid % numBlock;
        *lid = *lid / numBlock;
    }
}

// Please take care of the div ops
// Input the power of the size
Cache::Cache(int cacheSize,
             int blockSize,
             int numWay,
             int biasBitWidth) {

    int blocksPerLine = 1<<biasBitWidth;

    numLine     = numWay;
    numBlock    = blocksPerLine;

    int totalBlock    = cacheSize / blockSize;
    int totalLine     = totalBlock / blocksPerLine;
    numSet      = totalLine / numWay;

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

void Cache::print() {
    cout << "numBlock:\t" << numBlock << endl;
    cout << "numLine:\t" << numLine << endl;
    cout << "numSet: \t" << numSet << endl;
}

int Cache::search(int *idx, Addr addr) {
    CacheUnit bias = getBias(addr);
    CacheUnit tag = getTag(addr);
    CacheUnit set = getSet(addr);

    
}
