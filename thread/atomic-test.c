/*
 * Atomic functions test program
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

#include "atomic.h"

int main( void )
{
    int i = 10;
    int j = 20;

    j = atomicExchange( &i, 20 );
    printf("i = %d, j = %d\n", atomicLoad( &i ), atomicLoad( &j ));
    printf("i == 20 ? %d\n", atomicCompareExchange( &i, 10, 20 ));
    printf("i = %d\n", i );

    return 0;
}
