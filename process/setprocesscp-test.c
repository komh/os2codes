/*
 * functions to set a code page test program
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOS
#include <os2.h>

#include <stdio.h>
#include <string.h>
#include <process.h>

#include "setprocesscp.h"

#define SIGNATURE "!@#CHILD_SIGNATURE#@!"

int child()
{
    ULONG ulCurrentCp;
    ULONG cbCp;

    DosQueryCp( sizeof( ulCurrentCp ), &ulCurrentCp, &cbCp );

    printf("Child code page = %ld\n", ulCurrentCp );

    return 0;
}

int main( int argc, char *argv[])
{
    ULONG aulCp[ 3 ];
    ULONG cbCp;

    if( argc == 2 && !strcmp( argv[ 1 ], SIGNATURE ))
    {
        return child();
    }

    DosQueryCp( sizeof( aulCp ), aulCp, &cbCp );

    printf("Current code page = %ld\n", aulCp[ 0 ]);
    printf("Set a code page to %ld\n", aulCp[ 2 ]);

    setProcessCp( aulCp[ 2 ], CP_INHERIT );

    return spawnlp( P_WAIT, argv[ 0 ], argv[ 0 ], SIGNATURE, NULL );
}
