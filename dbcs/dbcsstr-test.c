/*
 * DBCS string functions test program
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>

#include "dbcsstr.h"

int main( int argc, char *argv[])
{
    const char *s;
    int i;

    if( argc < 2 )
    {
        fprintf(stderr, "Please specify any string !!!");

        return 1;
    }

    printf("DBCS enabled ? %s\n", dbcsIsEnabled() ? "YES" : "NO");

    s = argv[ 1 ];

    printf("Length of [%s] is %d\n", s, dbcsStrlen( s ));

    for( i = 0; s[ i ]; i++ )
    {
        if( dbcsIsLeadByte( s, i ))
        {
            printf("%3d: DBCS lead byte\n", i );

            /* Check if a broken a DBCS character. */
            if( s[ i + 1 ])
            {
                printf("%3d: DBCS trail byte\n", i + 1 );
                i++;
            }
        }
        else
            printf("%3d: SBCS\n", i );
    }

    return 0;
}

