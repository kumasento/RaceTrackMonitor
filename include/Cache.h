#ifndef _CACHE_H_
#define _CACHE_H_

#include <cstdint>

#include "Timer.h"

// tag bits couldn't be longer than 32bits
typedef uint32_t CacheUnit;

typedef uint32_t Addr;

struct CacheBlock {
    CacheUnit   valid;
    CacheUnit   tag;
    TimeType    access_time;

    inline CacheUnit getTag() { return tag; }
    inline CacheUnit getValid() { return valid; }
};

int shiftCount(unsigned u);

typedef int CacheIdx;
struct CacheDim3 {
    int s, l, b;
    CacheDim3(int s, int l, int b): s(s), l(l), b(b) {}
};

class Cache {
public:
    Cache() {}
    Cache(int cacheSize,
          int blockSize,
          int numWay,
          int biasBitWidth);

    void print_info();
    void print_status();

    // 2 kinds of cache locating method
    CacheIdx toIdx(CacheDim3 dim3);
    CacheDim3 toDim3(CacheIdx idx);

    CacheUnit getBias(Addr addr);
    CacheUnit getTag(Addr addr);
    CacheUnit getSet(Addr addr);

    // search for tag
    int     numBlock;   // blocks per line
    int     numLine;    // lines per set
    int     numSet;     // number of sets
    int     size;       // general size(num of blocks)

    // redundant information
    int     biasLen;
    int     setLen;

    // all these method use the CacheIdx
    // search for addr, if success, return idx, else -1
    CacheIdx search(Addr addr);

    // operation with timer
    Timer read(Addr addr, Timer timer);
    Timer write(Addr addr, Timer timer); // we assume the write content will not work
    Timer access(CacheIdx idx, Timer timer);
    
    CacheIdx replace(Addr addr, Timer timer); // return the allocated place

    int alloc(CacheIdx idx, CacheUnit tag, Timer timer);

private:
    CacheBlock* blocks;

    // address transform
    CacheUnit   biasmask;
    CacheUnit   setmask;
    CacheUnit   tagmask;
};


#endif 
