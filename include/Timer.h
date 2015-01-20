#ifndef _TIMER_H_
#define _TIMER_H_

typedef long long TimeType;

struct Timer {
    TimeType time;

    Timer() { time = 0; }
    Timer(TimeType t): time(t) {}
    void update(TimeType t) { time += t; }
};

#endif

