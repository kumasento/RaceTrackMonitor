
#include "RaceTrack.h"

#include <iostream>
#include <cmath>

using namespace std;

RTGroup::RTGroup(vector<RTPortPos> r, 
                 vector<RTPortPos> w,
                 vector<RTPortPos> rw) {
    head = 0;
    copy(this->r.begin(), this->r.end(), r.begin());
    copy(this->w.begin(), this->w.end(), w.begin());
    copy(this->rw.begin(), this->rw.end(), rw.begin());
}

RTPortPos RTGroup::findMinDis(RTPos pos, vector<RTPortPos> ports) {
    RTPos aligned = toStdPos(pos); 

    RTPos minPos;
    RTPos minDis = -1;
    
    for (int i = 0; i < w.size(); i++) {
        RTPortPos port = w[i];
        RTPos dis = abs(port - aligned);
        if (minDis == -1 || minDis > dis) {
            minDis = dis;
            minPos = port;
        }
    }

    return minPos;
}

RTPortPos RTGroup::findPort(RTPos pos, RTPortType type) {
    RTPos minPos;

    // For Read
    vector<RTPortPos> checked(rw);
    if (type == ReadPort) 
        checked.insert(checked.end(), r.begin(), r.end());
    else if (type == WritePort)
        checked.insert(checked.end(), w.begin(), w.end());

    minPos = findMinDis(pos, checked);

    return minPos;
}

RaceTrack::RaceTrack(int numDomain,
                     int numTrack,
                     int size,
                     TimeType shiftTime,
                     vector<RTPortPos> r,
                     vector<RTPortPos> w,
                     vector<RTPortPos> rw) {
    this->numDomain = numDomain;
    this->numTrack = numTrack;
    this->size = size; // size is bitwidth
    this->numGroup = size / (numTrack * numDomain);

    this->shiftTime = shiftTime;

    for (int i = 0; i < numGroup; i ++)
        groups.push_back(RTGroup(r, w, rw));
}

Timer RaceTrack::move(RTPos src, RTPos dst, int gid, Timer timer) {
    int nd = dst - src;
    int d = abs(nd);

    timer.update(shiftTime * d);
    groups[gid].head += nd;

    return timer;
}

Timer RaceTrack::access(RTPos pos, int gid, RTPortType type, Timer timer) {

    // always choose the nearest?
    if (gid < 0 || gid >= numGroup) {
        // exception
    }

    RTPos dst = groups[gid].findPort(pos, type);
    RTPos src = groups[gid].toStdPos(pos);

    timer = move(src, dst, gid, timer);

    // here should insert some move back method;

    return timer;
}

Timer RaceTrack::read(RTPos pos, int gid, Timer timer) {
    return access(pos, gid, ReadPort, timer);
}

Timer RaceTrack::write(RTPos pos, int gid, Timer timer) {
    return access(pos, gid, WritePort, timer);
}

