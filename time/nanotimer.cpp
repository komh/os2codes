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

#include "nanotimer.h"

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
