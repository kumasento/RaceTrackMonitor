
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "Scheduler.h"

using namespace std;

void Event::print() 
{
#ifdef DEBUG
    cout << "[Event] opcode: " 
         << ((op == Read) ? "READ" : "WRITE") 
         << " addr: "
         << addr
         << " tick: "
         << tick
         << endl;
#endif
}

// try with K&R braces
Scheduler::Scheduler(Monitor &monitor) 
{
    this->monitor = monitor;
    this->cur_timer = Timer(0);
}

Event Scheduler::parser(string opcode, string addr_str, string tick_str) 
{
    OpCode op;
    if (opcode == string("r"))
        op = Read;
    else if (opcode == string("w") || opcode == string("u"))
        op = Write;

    Addr addr = (Addr) atoll(addr_str.c_str());
    TimeType tick = (TimeType) atoll(tick_str.c_str());
    
    return Event(op, addr, tick);
}

void Scheduler::schedule(Event e) 
{
    // if the trace time is larger than the scheduler time
    // change time to trace time
    // or 
    // let trace wait

    TimeType trace_time = e.tick;
    TimeType cur_time = cur_timer.time;

    cur_timer.assign(max(trace_time, cur_time));

#ifdef DEBUG
    cout << "[Schedule] Task Scheduled: " << cur_timer.time << " ticks" << endl;
#endif
    if (e.op == Read) 
        cur_timer = monitor.cache_read(e.addr, cur_timer);
    else if (e.op == Write)
        cur_timer = monitor.cache_write(e.addr, cur_timer);
    else 
    {
        // Exception
    }


#ifdef DEBUG
    cout << "[Schedule] Task Finished: " << cur_timer.time << " ticks" << endl;
#endif
}

void Scheduler::run(string file_name) 
{
    fstream trace(file_name.c_str(), ios::in);

    while (!trace.eof()) {
        string opcode, addr_str, tick_str;
        trace >> opcode;
        trace >> addr_str;
        trace >> tick_str;
        trace >> tick_str;
        trace >> tick_str;

        if (opcode == "" || addr_str == "" || tick_str == "")
            break;

        // parse
        Event e = parser(opcode, addr_str, tick_str);
        e.print();

        schedule(e);
        
    }

    cout << cur_timer.time << endl;
}
