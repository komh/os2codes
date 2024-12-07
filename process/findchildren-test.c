/*
 * findchildren() test program
 *
 * Copyright (C) 2024 KO Myung-Hun <komh78@gmail.com>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>

#include "findchildren.h"

#define MAX_CHILDREN 10

int main( void )
{
    pid_t children[ MAX_CHILDREN ];
    int nchildren;
    int i;

    nchildren = findchildren( 0, children, MAX_CHILDREN );

    printf("# of children of PID 0 = %d\n", nchildren );

    if( nchildren > MAX_CHILDREN )
        nchildren = MAX_CHILDREN;

    printf("List of children up to %d\n", nchildren );

    for( i = 0; i < nchildren; i++ )
        printf("%dth child PID = %#x\n", i + 1, children[ i ]);

    return 0;
}
