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

#ifndef OS2CODES_NANOTIMER_H
#define OS2CODES_NANOTIMER_H

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
#endif
