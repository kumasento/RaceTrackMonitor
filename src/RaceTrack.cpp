
#include "RaceTrack.h"

#include <iostream>
#include <cmath>
#include <cstring>
#include <stack>

using namespace std;

RTGroup::RTGroup(vector<RTPortPos> r, 
                 vector<RTPortPos> w,
                 vector<RTPortPos> rw,
                 int local_size) {

    head = 0;
    memset(num_global, 0, sizeof(num_global));
    headPos_global = 0;
    headPosNum_global = 0;
    memset (queue_local, 0 ,sizeof (queue_local) );
    queue_f = 0; 
    queue_r = 0;
    queue_num = 0;
    queue_size = local_size;

    lastTime = 0;
    shiftTime = 1000;
    this->r.insert(this->r.end(), r.begin(), r.end());
    this->w.insert(this->w.end(), w.begin(), w.end());
    this->rw.insert(this->rw.end(), rw.begin(), rw.end());
}

RTPortPos RTGroup::judge() {
    stack<int> Stack;
    while (! Stack.empty()) Stack.pop();
    int f = queue_f, size = queue_size;

//    cout << queue_num << endl;
    for (int i = 0; i < queue_num; i ++) {
        if (Stack.empty()) {
            Stack.push(queue_local[(f + i) % size]);
        }
        else {
            if (Stack.top() == queue_local[(f + i) % size])
                Stack.push(queue_local[(f + i) % size]);
            else
            Stack.pop();
        }
    }
    if (! Stack.empty()) 
        return Stack.top(); 
    else 
        return GLOBAL;
}

void RTGroup::update(Timer timer){
    if (headPosNum_global == 0) 
        return;
    
    int choice = judge();
    if (choice == GLOBAL) choice = headPos_global;
 //   if (choice != GLOBAL) cout << "local: " << choice << endl;
    if (timer.time - lastTime > abs(head - choice) * shiftTime) {
	    head = choice;
	    return;
    }
    int step = (timer.time - lastTime) / shiftTime;
    if (choice > head) 
        head = head + step; 
    else 
        head = head - step;
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
                     int local_size,
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
        groups[i] = RTGroup(r, w, rw, local_size);
}

Timer RaceTrack::move(RTPos src, RTPos dst, int gid, Timer timer) {
    int nd = dst - src;
    int d = abs(nd);

    timer.update(shiftTime * d);

    groups[gid].head += nd;
    
   
    int head = groups[gid].head + 20;
    if (++ groups[gid].num_global[head] > groups[gid].headPosNum_global) {
        groups[gid].headPosNum_global = groups[gid].num_global[head];
        groups[gid].headPos_global = head - 20;
    }

    head = head - 20; 
    int size = groups[gid].queue_size;
    if (groups[gid].queue_num == size) 
    {
        groups[gid].queue_local[groups[gid].queue_f] = head;
        groups[gid].queue_f 
            = groups[gid].queue_r 
            = (groups[gid].queue_f + 1) % size;
    }
    else {
        groups[gid].queue_num ++;
        groups[gid].queue_local[groups[gid].queue_r] = head;
        groups[gid].queue_r = (groups[gid].queue_r + 1) % size;
    }
    
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

    groups[gid].update(timer);

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

