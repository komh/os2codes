/*
 * DosAliasMemEx() test program
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include "dosaliasmemex.h"

#define INCL_DOS
#include <os2.h>

#include <stdio.h>

#define ALLOC_SIZE  ( 8 * 1024 )
#define GAP_SIZE    ( 10 * 1024 * 1024 )

int main( void )
{
    char *p = NULL;
    char *pAlias1 = NULL;
    char *pAlias2 = NULL;
    int  fFailed = 1;
    ULONG rc;

    do
    {
        rc = DosAllocMem(( PPVOID )&p, ALLOC_SIZE, fPERM | PAG_COMMIT );
        fprintf( stderr, "DosAllocMem( p ) = %ld, p = %p\n", rc, p );
        if( rc )
            break;

        rc = DosAllocMem(( PPVOID )&pAlias1, GAP_SIZE, fPERM | PAG_COMMIT );
        fprintf( stderr, "DosAllocMem( pAlias1 ) = %ld, pAlias1 = %p\n",
                 rc, pAlias1 );
        if( rc )
            break;

        rc = DosAllocMem(( PPVOID )&pAlias2, ALLOC_SIZE, fPERM | PAG_COMMIT );
        fprintf( stderr, "DosAllocMem( pAlias2 ) = %ld, pAlias2 = %p\n",
                 rc, pAlias2 );
        if( rc )
            break;

        rc = DosFreeMem( pAlias2 );
        fprintf( stderr, "DosFreeMem( pAlias2 ) = %ld, pAlias2 = %p\n",
                 rc, pAlias2 );
        if( rc )
            break;

        rc = DosFreeMem( pAlias1 );
        fprintf( stderr, "DosFreeMem( pAlias1 ) = %ld, pAlias1 = %p\n",
                 rc, pAlias1 );
        if( rc )
            break;

        pAlias1 = pAlias2;
        pAlias2 = NULL;

        /* Try to allocate at the already allocated address */
        rc = DosAliasMemEx( p, ALLOC_SIZE / 2, ( PPVOID )&pAlias1,
                            OBJ_LOCATION );
        fprintf( stderr, "DosAliasMemEx( pAlias1 ) = %ld, pAlias1 = %p\n",
                 rc, pAlias1 );
        if( rc )
            break;

        rc = DosAliasMemEx( p, ALLOC_SIZE, ( PPVOID )&pAlias2, 0 );
        fprintf( stderr, "DosAliasMemEx( pAlias2 ) = %ld, pAlias2 = %p\n",
                 rc, pAlias2 );
        if( rc )
            break;

        *p = '!';
        fprintf( stderr, "*p = %c, *pAlias1 = %c, *pAlias2 = %c\n",
                 *p, *pAlias1, *pAlias2 );

        fFailed = ( *p != *pAlias1 ) || ( *p != *pAlias2 );
    } while( 0 );

    if( pAlias2 )
    {
        rc = DosFreeMem( pAlias2 );
        fprintf( stderr, "DosFreeMem( pAlias2 ) = %ld\, pAlias2 = %p\n",
                 rc, pAlias2 );
        fFailed |= rc;
    }

    if( pAlias1 )
    {
        rc = DosFreeMem( pAlias1 );
        fprintf( stderr, "DosFreeMem( pAlias1 ) = %ld, pAlias1 = %p\n",
                 rc, pAlias1 );
        fFailed |= rc;
    }

    if( p )
    {
        rc = DosFreeMem( p );
        fprintf(  stderr, "DosFreeMem( p ) = %ld, p = %p\n", rc, p );
        fFailed |= rc;
    }

    printf("%s\n", fFailed ? "FAILED" : "PASSED");

    return fFailed;
}
