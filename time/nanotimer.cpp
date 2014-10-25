/*
 * NanoTimer, a class to provide a nano-res timer for OS/2
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOS
#include <os2.h>

#include <cstdlib>

class NanoTimer
{
public:
    NanoTimer();
    ~NanoTimer();

    typedef long long int NanoClock;

    static const NanoClock NANO_TIME_BASE = 1000LL * 1000LL * 1000LL;

    NanoClock clock();

private:
    unsigned long _freq;
};

NanoTimer::NanoTimer()
{
    ULONG freq;

    DosTmrQueryFreq(&freq);

    _freq = freq;
}

NanoTimer::~NanoTimer()
{
}

NanoTimer::NanoClock NanoTimer::clock()
{
    union
    {
        long long ticks;
        QWORD qwTime;
    } t;

    DosTmrQueryTime(&t.qwTime);

    lldiv_t ns;

    ns = lldiv(t.ticks, _freq);

    return (ns.quot * NANO_TIME_BASE + (ns.rem * NANO_TIME_BASE) / _freq);
}

#include <iostream>

int main()
{
    NanoTimer nt;
    NanoTimer::NanoClock start;
    NanoTimer::NanoClock end;

    start = nt.clock();
    DosSleep(100);
    end = nt.clock();

    std::cout << "Start time = " << start << " ns\n";
    std::cout << "End time = " << end << " ns\n";
    std::cout << "Duration time = " << ((end - start) / 1000.f / 1000.f)
              << " ms\n";

    return 0;
}
