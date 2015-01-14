#ifndef _CACHE_H_
#define _CACHE_H_

#include <cstdint>

// tag bits couldn't be longer than 32bits
typedef uint32_t CacheUnit;

typedef uint32_t Addr;

struct CacheBlock {
    CacheUnit   valid;
    CacheUnit   tag;

    inline CacheUnit getTag() { return tag; }
    inline CacheUnit getValid() { return valid; }
};

class Cache {
public:
    Cache() {}
    Cache(int cacheSize,
          int blockSize,
          int numWay,
          int biasBitWidth);

    void print();

    void toIdx(int* idx, int* sid, int* lid, int* bid);
    void toDim3(int* idx, int *sid, int* lid, int* bid);
    CacheUnit getBias(Addr addr);
    CacheUnit getTag(Addr addr);
    CacheUnit getSet(Addr addr);

    // search for tag
    int     numBlock;   // blocks per line
    int     numLine;    // lines per set
    int     numSet;     // number of sets
    // search for addr, if success, return 

private:
    CacheBlock* blocks;

    // address transform
    CacheUnit   biasmask;
    CacheUnit   setmask;
    CacheUnit   tagmask;

};



#endif 
