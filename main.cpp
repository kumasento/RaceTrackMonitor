#include <iostream>

#include "Cache.h"

using namespace std;

int main(int argc, char *argv[]) {
    Cache cache(4 * 1024 * 1024, 64, 8, 0);

    Addr addr = (1<<15)-1;

    cout << cache.getBias(addr) << endl;
    cout << cache.getTag(addr) << endl;
    cout << cache.getSet(addr) << endl;

    return 0;
}
