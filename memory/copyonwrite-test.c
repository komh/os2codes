/*
 * COPY-ON-WRITE emulation test program
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
#include <stdlib.h>
#include <string.h>

#include "copyonwrite.h"

#define ALLOC_SIZE  8192
#define COPY_SIZE  ( ALLOC_SIZE / 2 )

void worker( void *arg )
{
    static const char *p1Str = "1234567890";
    static const char *p2Str = "1234512345";

    char *p1;
    char *p2;

    printf("tid = %d\n", _gettid());

    DosAllocMem(( PPVOID )&p1, ALLOC_SIZE, fPERM | PAG_COMMIT );

    strcpy( p1, p1Str);
    p2 = copyOnWrite( p1, COPY_SIZE );

    printf("p1 = %p, p2 = %p\n", p1, p2 );
    printf("p1 = [%s], p2 = [%s]\n", p1, p2 );

    printf("Copying %.5s to ( p2 + 5 )\n", p2Str );
    memcpy( p2 + 5, p2Str, 5 );

    printf("p1 = [%s], p2 = [%s]\n", p1, p2 );

    if( memcmp( p1, p1Str, strlen( p1Str ))
        || memcmp( p2, p2Str, strlen( p2Str )))
        fprintf( stderr, "Test failed\n");
    else
        fprintf( stderr, "Test succeeded\n");
}

int main( void )
{
    TID tid;

    tid = _beginthread( worker, NULL, 1024 * 1024, NULL );

    DosWaitThread( &tid, DCWW_WAIT );

    worker( NULL );

    return 0;
}
