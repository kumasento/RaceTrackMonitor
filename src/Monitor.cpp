
#include "Monitor.h"

Timer Monitor::access(Addr addr, MonitorOpType type, Timer timer) {
    if (type == ReadOp) 
        timer = cache.read(addr, timer);
    else if (type == WriteOp)
        timer = cache.write(addr, timer);

    return timer;
}

RTCoord Monitor::translate(CacheIdx idx) {
    CacheDim3 dim3 = cache.toDim3(idx);

    // configure like the baseline
    int range = rt.numDomain / cache.numLine;
    // I assure this will mod == 0
    int gid = dim3.s / range;
    int pos = (dim3.s % range) * cache.numLine + dim3.l;

    return RTCoord(pos, gid);
}

// Cache Methods
Timer Monitor::cache_read(Addr addr, Timer timer) {
    CacheIdx idx = cache.search(addr);

    if (idx == -1) {
        idx = cache.replace(addr, timer);
        // update cache content
        timer = rt_write(translate(idx), timer);
    }

    // read back result
    timer = rt_read(translate(idx), timer);
    
    return timer;
}
Timer Monitor::cache_write(Addr addr, Timer timer) {
    CacheIdx idx = cache.search(addr);

    if (idx == -1) {
        idx = cache.replace(addr, timer);
        // update cache content
        timer = rt_write(translate(idx), timer);
    }

    // write back result
    timer = rt_write(translate(idx), timer);
    
    return timer;
}
// RaceTrack Methods
Timer Monitor::rt_read(RTCoord crd, Timer timer) {
    return rt.read(crd.pos, crd.gid, timer);
}
Timer Monitor::rt_write(RTCoord crd, Timer timer) {
    return rt.write(crd.pos, crd.gid, timer);
}
