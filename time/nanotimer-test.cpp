/*
 * NanoTimer test program
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <iostream>

#include <cstdlib>

#include "nanotimer.h"

int main()
{
    NanoTimer nt;
    NanoTimer::NanoClock start;
    NanoTimer::NanoClock end;

    start = nt.clock();
    _sleep2(100);
    end = nt.clock();

    std::cout << "Start time = " << start << " ns\n";
    std::cout << "End time = " << end << " ns\n";
    std::cout << "Duration time = " << ((end - start) / 1000.f / 1000.f)
              << " ms\n";

    return 0;
}
