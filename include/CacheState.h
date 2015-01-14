#ifndef _CACHE_STATE_H_
#define _CACHE_STATE_H_

#include "Cache.h"

// It's a state-machine
// 1. Idle: currently not active
// 2. Comparing: searching for correct tag

typedef enum {
    CACHE_IDLE,
    CACHE_COMP
} CSType; // CacheStateType

typedef enum {
} CSException;

typedef enum {
 
} CSOpType;

struct CSOpCode {
    
};

class CacheState {
public:
    CacheState(Cache &reg_cache);
    
private:
    Cache cache;
    CSType state;
};

#endif
