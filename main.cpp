#include <iostream>
#include <cstdio>
#include <string>

#include "Scheduler.h"
#include "RaceTrack.h"
#include "Cache.h"
#include "Monitor.h"

using namespace std;

Addr addrGen(CacheUnit tag, CacheUnit set, CacheUnit bias, Cache &cache) 
{
    Addr addr = 0;
    addr |= bias;
    addr |= set << cache.biasLen;
    addr |= tag << cache.setLen;

    return addr;
}

int main(int argc, char *argv[]) 
{
    if (argc <= 1) 
    {
        cerr << "Usage: " << argv[0] << " [trace]" << endl;
        return 1;
    }

    string file_name = string(argv[1]);

    Cache cache(4 * 1024 * 1024, 512, 8, 0);

    cache.print_info();

    vector<RTPortPos> r(0), w(0), rw(4);
    rw[0] = 0;
    rw[1] = 16;
    rw[2] = 32;
    rw[3] = 48;
    RaceTrack rt(64, 512, 4 * 1024 * 1024, 1000, r, w, rw); 

    Monitor monitor(cache, rt);
    Scheduler schd(monitor);
    schd.run(file_name);

    return 0;
}
