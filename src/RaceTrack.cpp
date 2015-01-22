
#include "RaceTrack.h"

#include <iostream>
#include <cmath>

using namespace std;

RTGroup::RTGroup(vector<RTPortPos> r, 
                 vector<RTPortPos> w,
                 vector<RTPortPos> rw) {
    head = 0;
    
    this->r.insert(this->r.end(), r.begin(), r.end());
    this->w.insert(this->w.end(), w.begin(), w.end());
    this->rw.insert(this->rw.end(), rw.begin(), rw.end());
}

void RTGroup::move(RTPos src, RTPos dst) 
{
    int dist = dst - src;
    head += dist;
}

void RTGroup::print() 
{
#ifdef DEBUG
    cout << "[RaceTrack] Group stats:" << endl
         << "head:\t" << head << endl
         << "ports:" << endl;
    cout << "=>read: ";
    for (int i = 0; i < r.size(); i++) cout << r[i] << ' ';
    cout << endl;
    cout << "=>write: ";
    for (int i = 0; i < w.size(); i++) cout << w[i] << ' ';
    cout << endl;
    cout << "=>read/write: ";
    for (int i = 0; i < rw.size(); i++) cout << rw[i] << ' ';
    cout << endl;
#endif
}

// pos is track pos
RTPortPos RTGroup::findMinDis(RTPos pos, vector<RTPortPos> ports) {
    RTPos stdpos = toStdPos(pos);

    RTPos minPos;
    RTPos minDis = -1;

    // get the average distance between ports
    sort(ports.begin(), ports.end());
    RTPos avg = 0;
    for (int i = 1; i < ports.size(); i++)
        avg += abs(ports[i] - ports[i-1]);
    avg /= (ports.size()-1);

    for (int i = 0; i < ports.size(); i++) {
        RTPortPos port = ports[i];
        RTPos dis = abs(port - stdpos);

#ifndef NOHEADRESTRICT 
        if (abs(head + port - stdpos) > avg)
            continue;
#endif
 
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
    
    groups.resize(numGroup);

    for (int i = 0; i < numGroup; i ++)
        groups[i] = RTGroup(r, w, rw);
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

#ifdef DEBUG
    cout << "[Monitor] RaceTrack Access" 
         << " track head: " << groups[gid].head
         << " pos: " << pos
         << " gid: " << gid
         << " size: " << groups.size()
         << " time: " << timer.time
         << endl;
#endif
    // Here pos is the Track position
    // findPort should take track as param

    RTPos dst = groups[gid].findPort(pos, type);
    RTPos src = groups[gid].toStdPos(pos);

    timer = move(src, dst, gid, timer);
    // here should insert some move back method;
#ifdef DEBUG
    cout << "[Monitor] RaceTrack Access" 
         << " track head: " << groups[gid].head
         << " dst: " << dst
         << " src: " << src
         << " time: " << timer.time
         << endl;
#endif

    return timer;
}

Timer RaceTrack::read(RTPos pos, int gid, Timer timer) {
    return access(pos, gid, ReadPort, timer);
}

Timer RaceTrack::write(RTPos pos, int gid, Timer timer) {
    return access(pos, gid, WritePort, timer);
}

