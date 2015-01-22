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
    if (argc <= 2) 
    {
        cerr << "Usage: " << argv[0] << " [trace] [local size]" << endl;
        return 1;
    }

    string file_name = string(argv[1]);
    int local_size = atoi(argv[2]);

    Cache cache(4 * 1024 * 1024, 512, 8, 0);

    cache.print_info();

    vector<RTPortPos> r(8), w(4), rw(0);
  //  rw[0] = 0;
  //  rw[1] = 16;
   // rw[2] = 32;
  //  rw[3] = 48;
    r[0] = 12;
    r[1] = 28;
    r[2] = 44;
    r[3] = 60;
    r[4] = 0;
    r[5] = 16;
    r[6] = 32;
    r[7] = 48;
    w[0] = 4;
    w[1] = 20;
    w[2] = 36;
    w[3] = 52;
    RaceTrack rt(64, 512, 4 * 1024 * 1024, local_size, 1000, r, w, rw); 

    Monitor monitor(cache, rt);
    Scheduler schd(monitor);
    schd.run(file_name);

    return 0;
}
