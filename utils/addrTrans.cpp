#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <bitset>

#include "Cache.h"

using namespace std;

string getBit32(unsigned n) {
    bitset<32> bit32(n);
    return bit32.to_string();
}


int main(int argc, char *argv[]) {
    int cacheSize;
    int blockSize;
    int numWay;
    int biasBitWidth;

    unsigned addr;

    if (argc <= 4) {
        fprintf(stderr,
                "Usage: %s [cacheSize(MB)] [blockSize(B)] [numWay] [biasbits]\n",
                argv[0]);
        exit(1);
    }

    cacheSize = atoi(argv[1]);
    blockSize = atoi(argv[2]);
    numWay = atoi(argv[3]);
    biasBitWidth = atoi(argv[4]);
    
    cacheSize = cacheSize << 20;

    Cache cache(cacheSize, blockSize, numWay, biasBitWidth);

    while(!cin.eof()) {
        cout << "addr:" << endl;
        cin >> addr;

        cout << "Binary:\t0x" << getBit32(addr) << endl;
        
        unsigned tag = cache.getTag(addr);
        unsigned set = cache.getSet(addr);
        unsigned bias = cache.getBias(addr);

        cout << "=>Tag:\t0x" << getBit32(tag) << endl;
        cout << "=>Set:\t0x" << getBit32(set) << endl;
        cout << "=>Bias:\t0x" << getBit32(bias) << endl;
    }
}
