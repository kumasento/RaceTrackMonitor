#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "Cache.h"
#include "RaceTrack.h"
#include "Timer.h"

typedef enum {
    ReadOp,
    WriteOp
} MonitorOpType;

class Monitor {
public:
    Monitor(Cache &cache, RaceTrack &rt) {
        this->cache = cache;
        this->rt = rt;
    }

    Timer access(Addr addr, MonitorOpType type, Timer timer);

    // Cache Methods
    Timer cache_read(Addr addr, Timer timer);
    Timer cache_write(Addr addr, Timer timer);
    // RaceTrack Methods
    Timer rt_read(RTCoord crd, Timer timer);
    Timer rt_write(RTCoord crd, Timer timer);

    RTCoord translate(CacheIdx idx);

private:
    Cache cache;
    RaceTrack rt;


};

#endif
