
#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>

Cache buildCacheFromCmd(int argc, char *argv[]) {
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

    return cache;
}

#endif
