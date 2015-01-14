#include <iostream>
#include <cstdio>

#include "Cache.h"
#include "CacheState.h"

using namespace std;

int main(int argc, char *argv[]) {
    Cache cache(4 * 1024 * 1024, 64, 8, 0);

    Addr addr = (1<<15)-1;

    cout << cache.getBias(addr) << endl;
    cout << cache.getTag(addr) << endl;
    cout << cache.getSet(addr) << endl;

    cache.print();

    int idx, sid, lid, bid;
    sid = 2;
    lid = 1;
    bid = 0;
    cache.toIdx(&idx, &sid, &lid, &bid);
    printf("(%d, %d, %d) => %d\n", sid, lid, bid, idx);
    cache.toDim3(&idx, &sid, &lid, &bid);
    printf("%d => (%d, %d, %d)\n", idx, sid, lid, bid);

    CacheState cstate(cache);

    return 0;
}
