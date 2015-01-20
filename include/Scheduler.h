#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "Monitor.h"
#include "Timer.h"

#include <string>
#include <iostream>

using namespace std;

typedef enum 
{
    Read,
    Write
} OpCode;

struct Event 
{
    OpCode op;
    Addr addr;
    TimeType tick;

    Event(OpCode op, Addr addr, TimeType tick): op(op), addr(addr), tick(tick) {}
    void print();
};

class Scheduler 
{
public:
    Scheduler(Monitor &monitor); 

    void run(string file_name);
    void schedule(Event e);
    Event parser(string opcode, string addr_str, string tick_str);
private:

    // record the current time
    Timer cur_timer;
    Monitor monitor;
};

#endif
