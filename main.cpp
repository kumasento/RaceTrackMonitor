#include <iostream>
#include <cstdio>

#include "RaceTrack.h"
#include "Cache.h"
#include "Monitor.h"

using namespace std;

Addr addrGen(CacheUnit tag, CacheUnit set, CacheUnit bias, Cache &cache) {
    Addr addr = 0;
    addr |= bias;
    addr |= set << cache.biasLen;
    addr |= tag << cache.setLen;

    return addr;
}

int main(int argc, char *argv[]) {
    Cache cache(4 * 1024 * 1024, 64, 8, 0);

    Addr addr = (1<<15)-1;

    cout << cache.getBias(addr) << endl;
    cout << cache.getTag(addr) << endl;
    cout << cache.getSet(addr) << endl;

    cache.print_info();

    CacheUnit tag = cache.getTag(addr);
    CacheUnit set = cache.getSet(addr);
    CacheUnit bias = cache.getBias(addr);

    cout << addr << "->" << addrGen(tag, set, bias, cache) << endl;

    Timer timer = Timer(0);

    vector<RTPortPos> r(0), w(0), rw(4);
    rw[0] = 0;
    rw[1] = 16;
    rw[2] = 32;
    rw[3] = 48;
    RaceTrack rt(64, 512, 4 * 1024 * 1024, 1, r, w, rw); 

    timer = rt.read(7, 0, timer);
    cout << timer.time << endl;
    timer = rt.read(8, 0, timer);
    cout << timer.time << endl;
    timer = rt.read(9, 0, timer);
    cout << timer.time << endl;
    timer = rt.read(16, 0, timer);
    cout << timer.time << endl;

    Monitor monitor(cache, rt);
    
    for (int n = 0; n < 2; n++)
        for (int i = 0; i < cache.numLine - 1; i++) {
            Addr ta = addrGen(tag+i, set, bias, cache);

            timer = monitor.cache_read(ta, timer);

            cout << timer.time << endl;
        }


    return 0;
}
