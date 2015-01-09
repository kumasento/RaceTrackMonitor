#ifndef _CACHEGENERATOR_H_
#define _CACHEGENERATOR_H_

typedef int CacheBlockId;
typedef int CacheLineId;
typedef int CacheSetId;

typedef int CacheBlockNum;
typedef int CacheLineNum;
typedef int CacheSetNum;
typedef int CacheNum;

typedef bool CacheBit;

typedef uint8_t     CacheTag8;
typedef uint16_t    CacheTag16;
typedef unsigned    CacheTag32;
typedef CacheTag32  CacheTag;

struct CacheBlock {
    CacheBlockId id;
};

struct CacheLine {
    // CacheLine Info
    CacheBit        valid;
    CacheTag        tag;
    CacheBlockNum   num;
    CacheLineId     id;

    // CacheLine Data
    CacheBlock*     blocks;   
};

struct CacheSet {
    CacheSetId      id;
    CacheLineNum    num;
    CacheLine*      lines;
};

struct Cache {
    CacheSetNum    num;
    CacheSet*   sets;

    CacheTag    tagmask;
};

#endif
