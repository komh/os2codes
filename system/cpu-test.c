/*
 * CPU functions test program
 *
 * Copyright (C) 2014-2024 KO Myung-Hun <komh@chollian.net>
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

    uint64_t *pAccDelta;
    uint64_t *pAccIdle;
    uint64_t *pAccBusy;
    uint64_t *pAccIntr;

    float *pMinCpuLoad;
    float *pMaxCpuLoad;

    float *pMinIntrLoad;
    float *pMaxIntrLoad;

    int nCpus;
    int i;

    nCpus = getCpuCount();

    printf("Total CPU count = %d\n", nCpus );
    printf("Total workable CPU count = %d\n", getCpuOnCount());

    cbCpuTime = sizeof( *pctPrev ) * nCpus ;

    pctPrev    = calloc( nCpus, sizeof( *pctPrev ));
    pctCurrent = calloc( nCpus, sizeof( *pctCurrent ));

    pAccDelta = calloc( nCpus, sizeof( *pAccDelta ));
    pAccIdle  = calloc( nCpus, sizeof( *pAccIdle ));
    pAccBusy  = calloc( nCpus, sizeof( *pAccBusy ));
    pAccIntr  = calloc( nCpus, sizeof( *pAccIntr ));

    pMinCpuLoad = calloc( nCpus, sizeof( *pMinCpuLoad ));
    pMaxCpuLoad = calloc( nCpus, sizeof( *pMaxCpuLoad ));

    pMinIntrLoad = calloc( nCpus, sizeof( *pMinIntrLoad ));
    pMaxIntrLoad = calloc( nCpus, sizeof( *pMaxIntrLoad ));

    printf("Press any key to quit.\n");

    do
    {
        if( getCpuTime( pctCurrent, cbCpuTime ))
            break;

        if( pctPrev[ 0 ].time > 0 )
        {
            uint64_t delta;
            uint64_t idle;
            uint64_t busy;
            uint64_t intr;

            float load;

            int j;

            for( j = nCpus - 1; j >= 0; j-- )
            {
                delta = pctCurrent[ j ].time - pctPrev[ j ].time;
                idle  = pctCurrent[ j ].idle - pctPrev[ j ].idle;
                busy  = pctCurrent[ j ].busy - pctPrev[ j ].busy;
                intr  = pctCurrent[ j ].intr - pctPrev[ j ].intr;

                pAccDelta[ j ] += delta;
                pAccIdle[ j ]  += idle;
                pAccBusy[ j ]  += busy;
                pAccIntr[ j ]  += intr;

                load = ( float )busy / delta * 100;
                if( pMinCpuLoad[ j ] == 0.0f || pMinCpuLoad[ j ] > load )
                    pMinCpuLoad[ j ] = load;
                if( pMaxCpuLoad[ j ] < load )
                    pMaxCpuLoad[ j ] = load;

                load = ( float )intr / delta * 100;
                if( pMinIntrLoad[ j ] == 0.0f || pMinIntrLoad[ j ] > load )
                    pMinIntrLoad[ j ] = load;
                if( pMaxIntrLoad[ j ] < load )
                    pMaxIntrLoad[ j ] = load;
            }

            printf("CPU0: Busy = %5.1f%%, Interrupt = %5.1f%%, Idle = %5.1f%%\r",
                   ( float )busy / delta * 100,
                   ( float )intr / delta * 100,
                   ( float )idle / delta * 100 );
        }

        memcpy( pctPrev, pctCurrent, cbCpuTime );
        _sleep2( 1000 );
    } while( _read_kbd( 0, 0, 0 ) == -1 );

    if( pAccDelta[ 0 ] > 0 )
    {
        printf("CPU    CPU Max    CPU Min    CPU Avg    INT Max    INT Min    INT Avg\n");
        for( i = 0; i < nCpus; i++ )
        {
            printf("CPU%-2d   %5.1f      %5.1f      %5.1f      %5.1f      %5.1f      %5.1f\n",
                   i,
                   pMaxCpuLoad[ i ], pMinCpuLoad[ i ],
                   ( float )pAccBusy[ i ] / pAccDelta[ i ] * 100,
                   pMaxIntrLoad[ i ], pMinIntrLoad[ i ],
                   ( float )pAccIntr[ i ] / pAccDelta[ i ] * 100 );
        }
    }

    free( pMaxIntrLoad );
    free( pMinIntrLoad );

    free( pMaxCpuLoad );
    free( pMinCpuLoad );

    free( pAccIntr );
    free( pAccBusy );
    free( pAccIdle );
    free( pAccDelta );

    free( pctCurrent );
    free( pctPrev );

    return 0;
}
