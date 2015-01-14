
#include "CacheState.h"

// reg means register
CacheState::CacheState(Cache &reg_cache) {
    // initialized with idle
    state = CACHE_IDLE;
    // initialized with an empty cache
    cache = reg_cache; 
}


