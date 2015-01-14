#include <iostream>
#include <cstdio>

#include "Cache.h"
#include "Utils.h"

using namespace std;


int main(int argc, char *argv[]) {
    Cache cache = buildCacheFromCmd(argc, argv);

    cache.print();

    while (!cin.eof()) {
        int idx, sid, lid, bid;
        cout << "idx => (sid, lid, bid):\t";
        cin >> idx;
        cache.toDim3(&idx, &sid, &lid, &bid);
        printf("%3d => (%3d, %3d, %3d)\n", idx, sid, lid, bid);

        cout << "(sid, lid, bid) => idx:\t";
        cin >> sid >> lid >> bid;
        cache.toIdx(&idx, &sid, &lid, &bid);
        printf("(%3d, %3d, %3d) => %3d\n", sid, lid, bid, idx);
    }

    return 0;
}
