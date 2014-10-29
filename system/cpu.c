/*
 * CPU functions
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
#define INCL_DOSSPINLOCK
#include <os2.h>

#include <errno.h>

#include "cpu.h"

int getCpuCount( void )
{
    ULONG ulCpus;

    if( DosQuerySysInfo( QSV_NUMPROCESSORS, QSV_NUMPROCESSORS,
                         &ulCpus, sizeof( ulCpus )))
        ulCpus = 1;

    return ulCpus;
}

int getCpuOnCount( void )
{
    ULONG ulProcId;
    ULONG ulCpuOnCount;
    ULONG ulStatus;
    ULONG rc;

    for( ulProcId = 1, ulCpuOnCount = 0; ; ulProcId++ )
    {
        rc = DosGetProcessorStatus( ulProcId, &ulStatus );
        if( rc )
            break;

        if( ulStatus == PROC_ONLINE )
            ulCpuOnCount++;
    }

    return ulCpuOnCount == 0 ? 1 : ulCpuOnCount;
}

int getCpuTime( PCPUTIME pCpuTime, size_t cbSize )
{
    static ULONG ulCpuCount = ( ULONG )-1;

    if( ulCpuCount == ( ULONG )-1)
    {
        /* Query CPU count */
        if( DosPerfSysCall( 0x41, 0, ( ULONG )&ulCpuCount, 0 ))
            ulCpuCount = 1;
    }

    if( cbSize < sizeof( CPUTIME ) * ulCpuCount )
    {
        errno = EOVERFLOW;

        return -1;
    }

    /* Query CPU utilization snapshot */
    if( DosPerfSysCall( 0x63, ( ULONG )pCpuTime, 0, 0 ))
    {
        errno = EINVAL;

        return -1;
    }

    return 0;
}
