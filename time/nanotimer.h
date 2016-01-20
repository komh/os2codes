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

/** @file nanotimer.h */

#ifndef OS2CODES_NANOTIMER_H
#define OS2CODES_NANOTIMER_H

/**
 * @brief Class implementing nano timer
 */
class NanoTimer
{
public:
    /** Constructor */
    NanoTimer();
    /** Destructor */
    ~NanoTimer();

    /** NanoClock type */
    typedef long long int NanoClock;

    /** 1 second in nano seconds */
    static const NanoClock NANO_TIME_BASE = 1000LL * 1000LL * 1000LL;

    /**
     * @brief Get a nano clock snapshot from IRQ0 high resolution timer
     * (Intel 8254)
     * @return A nano clock snapshot
     */
    NanoClock clock();

private:
    /** Frequency of IRQ0 high resolution timer (Intel 8254) */
    unsigned long _freq;
};
#endif
