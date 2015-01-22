
#include <iostream>
#include <vector>

#include "RaceTrack.h"

using namespace std;

int main() {
    int n = 4;
    vector<RTPortPos> r(0), w(0), rw(n);
    rw[0] = 0;
    rw[1] = 16;
    rw[2] = 32;
    rw[3] = 48;
    RTGroup group(r, w, rw);

    vector<RTPos> testcases;

    for (int i = 16; i >= 0; i--)
        testcases.push_back(i);
    
    group.print();
    for (int i = 0; i < testcases.size(); i++) {
        cout << testcases[i] << endl;

        RTPos readpos = group.toStdPos(testcases[i]);
        RTPos portpos = group.findPort(testcases[i], ReadPort);

        cout << readpos << " => " << portpos << endl;

        // for example, if I want to read position 1 on track
        // and the nearest port is 0
        // then I should move the track -1 "pos"
        // the src is readpos and the dst is portpos
        //
        // what if the port is on 1 and I need to read 0?
        // 0 is the position on track
        // 1 is the standard postion
        //
        // so I need to make sure the ports postion is "standard"
        group.move(readpos, portpos);

        group.print();
    }

    return 0;
}
