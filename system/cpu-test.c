/*
 * CPU functions test program
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

int main( void )
{
    PCPUTIME pctPrev;
    PCPUTIME pctCurrent;
    int cbCpuTime;

    int nCpus;
    int i;

    nCpus = getCpuCount();

    printf("Total CPU count = %d\n", nCpus );
    printf("Total workable CPU count = %d\n", getCpuOnCount());

    cbCpuTime = sizeof( *pctPrev ) * nCpus ;

    pctPrev    = malloc( cbCpuTime );
    pctCurrent = malloc( cbCpuTime );

    for( i = 0; i < 2; i++ )
    {
        if( getCpuTime( pctCurrent, cbCpuTime ))
            break;

        if( i > 0 )
        {
            uint64_t delta;
            uint64_t idle;
            uint64_t busy;
            uint64_t intr;

            int j;

            for( j = 0; j < nCpus; j++ )
            {
                delta = pctCurrent[ j ].time - pctPrev[ j ].time;
                idle  = pctCurrent[ j ].idle - pctPrev[ j ].idle;
                busy  = pctCurrent[ j ].busy - pctPrev[ j ].busy;
                intr  = pctCurrent[ j ].intr - pctPrev[ j ].intr;

                printf("CPU%d: Idle = %.1f%%, Busy = %.1f%%, Interrupt = %.1f%%\n",
                       j, ( float )idle / delta * 100,
                       ( float )busy / delta * 100,
                       ( float )intr / delta * 100 );
            }
        }

        memcpy( pctPrev, pctCurrent, cbCpuTime );
        _sleep2( 100 );
    }

    free( pctCurrent );
    free( pctPrev );

    return 0;
}
