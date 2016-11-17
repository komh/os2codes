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

static void test1( void )
{
    static const char *p1Str = "1234567890";
    static const char *p2Str = "1234512345";

    char *p1;
    char *p2;

    printf("***** Test1: Copy to destination memory...\n");

    DosAllocMem(( PPVOID )&p1, ALLOC_SIZE, fPERM | PAG_COMMIT );

    strcpy( p1, p1Str);
    p2 = copyOnWrite( p1, COPY_SIZE );

    printf("p1 = %p, p2 = %p\n", p1, p2 );
    printf("p1 = [%s], p2 = [%s]\n", p1, p2 );

    printf("Copying %.5s to ( p2 + 5 )\n", p2Str );
    memcpy( p2 + 5, p2Str, 5 );

    printf("p1 = [%s], p2 = [%s]\n", p1, p2 );

    fprintf( stderr, "Test1: ");
    if( memcmp( p1, p1Str, strlen( p1Str ))
        || memcmp( p2, p2Str, strlen( p2Str )))
        fprintf( stderr, "Failed\n");
    else
        fprintf( stderr, "Succeeded\n");

    DosFreeMem( p2 );
    DosFreeMem( p1 );
}

static void test2( void )
{
    static const char *p1Str = "1234512345";
    static const char *p2Str = "1234567890";

    char *p1;
    char *p2;

    printf("***** Test2: Copy to source memory...\n");

    DosAllocMem(( PPVOID )&p1, ALLOC_SIZE, fPERM | PAG_COMMIT );

    strcpy( p1, p2Str);
    p2 = copyOnWrite( p1, COPY_SIZE );

    printf("p1 = %p, p2 = %p\n", p1, p2 );
    printf("p1 = [%s], p2 = [%s]\n", p1, p2 );

    printf("Copying %.5s to ( p1 + 5 )\n", p1Str );
    memcpy( p1 + 5, p1Str, 5 );

    printf("p1 = [%s], p2 = [%s]\n", p1, p2 );

    fprintf( stderr, "Test2: ");
    if( memcmp( p1, p1Str, strlen( p1Str ))
        || memcmp( p2, p2Str, strlen( p2Str )))
        fprintf( stderr, "Failed\n");
    else
        fprintf( stderr, "Succeeded\n");

    DosFreeMem( p2 );
    DosFreeMem( p1 );
}

static void test3( void )
{
    printf("***** Test3: Copy to source memory which is destination memory "
           "of other source memory...\n");

    static const char *p1Str = "6789067890";
    static const char *p2Str = "1234512345";
    static const char *p3Str = "1234567890";

    char *p1;
    char *p2;
    char *p3;

    DosAllocMem(( PPVOID )&p1, ALLOC_SIZE, fPERM | PAG_COMMIT );

    strcpy( p1, p3Str);
    p2 = copyOnWrite( p1, COPY_SIZE );
    p3 = copyOnWrite( p2, COPY_SIZE );

    printf("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3 );
    printf("p1 = [%s], p2 = [%s], p3 = [%s]\n", p1, p2, p3 );

    printf("Copying %.5s to ( p2 + 5 )\n", p2Str );
    memcpy( p2 + 5, p2Str, 5 );

    printf("Copying %.5s to p1\n", p1 + 5 );
    memcpy( p1, p1 + 5, 5 );

    printf("p1 = [%s], p2 = [%s], p3 = [%s]\n", p1, p2, p3 );

    fprintf( stderr, "Test3: ");
    if( memcmp( p1, p1Str, strlen( p1Str ))
        || memcmp( p2, p2Str, strlen( p2Str ))
        || memcmp( p3, p3Str, strlen( p3Str )))
        fprintf( stderr, "Failed\n");
    else
        fprintf( stderr, "Succeeded\n");

    DosFreeMem( p3 );
    DosFreeMem( p2 );
    DosFreeMem( p1 );
}

static void test4( void )
{
    printf("***** Test4: Free source memory...\n");

    static const char *p2Str = "6789067890";
    static const char *p3Str = "1234567890";

    char *p1;
    char *p2;
    char *p3;

    DosAllocMem(( PPVOID )&p1, ALLOC_SIZE, fPERM | PAG_COMMIT );

    strcpy( p1, p3Str);
    p2 = copyOnWrite( p1, COPY_SIZE );
    p3 = copyOnWrite( p2, COPY_SIZE );

    printf("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3 );
    printf("p1 = [%s], p2 = [%s], p3 = [%s]\n", p1, p2, p3 );

    printf("Freeing p1\n");
    printf("DosFreeMem( p1 ) = %ld\n", DosFreeMem( p1 ));

    printf("Copying %.5s to p2\n", p2 + 5 );
    memcpy( p2, p2 + 5, 5 );

    printf("p2 = [%s], p3 = [%s]\n", p2, p3 );

    fprintf( stderr, "Test4: ");
    if( memcmp( p2, p2Str, strlen( p2Str ))
        || memcmp( p3, p3Str, strlen( p3Str )))
        fprintf( stderr, "Failed\n");
    else
        fprintf( stderr, "Succeeded\n");

    DosFreeMem( p3 );
    DosFreeMem( p2 );
}

static void test5( void )
{
    printf("***** Test5: Free source memory which is destination memory "
           "of other source memory...\n");

    static const char *p1Str = "6789067890";
    static const char *p3Str = "1234567890";

    char *p1;
    char *p2;
    char *p3;

    DosAllocMem(( PPVOID )&p1, ALLOC_SIZE, fPERM | PAG_COMMIT );

    strcpy( p1, p3Str);
    p2 = copyOnWrite( p1, COPY_SIZE );
    p3 = copyOnWrite( p2, COPY_SIZE );

    printf("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3 );
    printf("p1 = [%s], p2 = [%s], p3 = [%s]\n", p1, p2, p3 );

    printf("Freeing p2\n");
    printf("DosFreeMem( p2 ) = %ld\n", DosFreeMem( p2 ));

    printf("Copying %.5s to p1\n", p1 + 5 );
    memcpy( p1, p1 + 5, 5 );

    printf("p1 = [%s], p3 = [%s]\n", p1, p3 );

    fprintf( stderr, "Test5: ");
    if( memcmp( p1, p1Str, strlen( p1Str ))
        || memcmp( p3, p3Str, strlen( p3Str )))
        fprintf( stderr, "Failed\n");
    else
        fprintf( stderr, "Succeeded\n");

    DosFreeMem( p3 );
    DosFreeMem( p1 );
}

static void worker( void *arg )
{
    printf("Testing in tid = %d\n", _gettid());

    test1();
    test2();
    test3();
    test4();
    test5();
}

int main( void )
{
    TID tid;

    tid = _beginthread( worker, NULL, 1024 * 1024, NULL );

    DosWaitThread( &tid, DCWW_WAIT );

    worker( NULL );

    return 0;
}
