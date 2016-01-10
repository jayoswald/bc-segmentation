#pragma once
#include <time.h>


class Timer {
    //static const int MY_CLOCK = CLOCK_THREAD_CPUTIME_ID;
    static const int MY_CLOCK = CLOCK_REALTIME;
public:
    Timer() {
        clock_gettime(MY_CLOCK, &_t0);
    }
    double elapsed() const {
        timespec _t1;
        clock_gettime(MY_CLOCK, &_t1);
        double dt = double(_t1.tv_sec - _t0.tv_sec);
        dt += double(_t1.tv_nsec - _t0.tv_nsec) * 1e-9;
        return dt;
    }

private:
    timespec _t0;
};

