#include <iostream>
#include <cstdlib>

#include "Cache.h"

using namespace std;

int shiftCount(unsigned u) {
    int count = 0;
    for (; count < 32 && (u & 1) == 0; count ++, u >>= 1);

    return count;
}

CacheIdx Cache::toIdx(CacheDim3 dim3) {
    CacheIdx idx;
    if (dim3.b >= numBlock || dim3.l >= numLine || dim3.s >= numSet)
        idx = -1;
    else
        idx =   dim3.b + 
                dim3.l * numBlock + 
                dim3.s * numBlock * numLine;
    return idx;
}
CacheDim3 Cache::toDim3(CacheIdx idx) {
    // if idx is invalid or idx is above boundary
    int s, l, b;
    if (idx == -1 || idx > numBlock * numLine * numSet)
        s = l = b = -1;
    else {
        s = idx / (numBlock * numLine);
        l = idx % (numBlock * numLine);
        b = l % numBlock;
        l = l / numBlock;
    }
    return CacheDim3(s, l, b);
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

    biasLen  = biasBitWidth;
    setLen   = shiftCount(numSet) + biasBitWidth;

    size     = numLine * numBlock * numSet;

    blocks   = (CacheBlock*) malloc(sizeof(CacheBlock)*size);

    biasmask = (1<<biasBitWidth)-1;
    setmask  = (numSet-1) << biasBitWidth;
    tagmask  = ~(biasmask | setmask);
}

CacheUnit Cache::getBias(Addr addr) {
    return biasmask & addr;
}

CacheUnit Cache::getSet(Addr addr) {
    return (setmask & addr) >> biasLen;
}

CacheUnit Cache::getTag(Addr addr) {
    return (tagmask & addr) >> setLen;
}

void Cache::print_info() {
    cout << "numBlock:\t" << numBlock << endl;
    cout << "numLine:\t" << numLine << endl;
    cout << "numSet: \t" << numSet << endl;
}

// I don't know whether should this function contains the write-back step
int Cache::alloc(CacheIdx idx, CacheUnit tag, Timer timer) {
    blocks[idx].valid = 1;
    blocks[idx].tag = tag;
    blocks[idx].access_time = timer.time; // initialize time

    return 1;
}

CacheIdx Cache::search(Addr addr) {
    CacheUnit set, tag, bias;

    tag = getTag(addr);
    set = getSet(addr);
    bias = getBias(addr);

    CacheIdx idx = -1; 
    // search in set
    for (int i = 0; i < numLine; i++) 
        for (int j = 0; j < numBlock; j++) {
            idx = toIdx(CacheDim3(set, i, j));
            if (idx != -1) { 
                if (blocks[idx].valid && blocks[idx].tag == tag) 
                    return idx;
            }
            else {
                // exception
            }
        }
    return -1;
}

CacheIdx Cache::replace(Addr addr, Timer timer) {
    CacheUnit set, tag;

    tag = getTag(addr);
    set = getSet(addr);

    CacheIdx minIdx = toIdx(CacheDim3(set, 0, 0));
    TimeType minTime = blocks[minIdx].access_time;

    // find the minimal time
    // GODDAMN NAIVE
    for (int i = 0; i < numLine; i++)
        for (int j = 0; j < numBlock; j++) {
            CacheIdx idx = toIdx(CacheDim3(set, i, j));
            TimeType t = blocks[idx].access_time;
            if (minTime > t) {
                minTime = t;
                minIdx = idx;
            }
        }

    // cout << "replaced tag: " << blocks[minIdx].tag << endl;

    alloc(minIdx, tag, Timer(minTime));

    return minIdx;
}

Timer Cache::access(CacheIdx idx, Timer timer) {
    // do the racetrack
    //


    blocks[idx].access_time = timer.time; // update the latest access time

    return timer;
}

Timer Cache::read(Addr addr, Timer timer) {
    CacheIdx idx = search(addr);

    if (idx == -1) {
        cout << "Miss" << endl;
        idx = replace(addr, timer);     
    }
    else
        cout << "Hit" << endl;

    // at this place, should call the RaceTrack function 
    // shift will take time
    // this is a wrapper: access
    timer = access(idx, timer);
    
    return timer;
}

