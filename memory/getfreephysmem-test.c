/*
 * getFreePhysMem() test program
 *
 * Copyright (C) 2021 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>

#include "getfreephysmem.h"

int main( void )
{
    printf("Free physical memory = %d MiB\n",
           getFreePhysMem() / 1024 / 1024 );

    return 0;
}
