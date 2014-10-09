/*
 * DosGetErrorMessage(): get an error message from an error code
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
#include <os2.h>

#include "dosgeterrormessage.h"

const char *DosGetErrorMessage( int errnum, char *buffer, size_t n )
{
    char *p1;
    char *p2;
    ULONG ulMsgLen;
    ULONG rc;

    rc = DosGetMessage( NULL, 0, buffer, n, errnum, "OSO001.MSG", &ulMsgLen );

    if( rc )
        return NULL;

    buffer[ ulMsgLen ] = '\0';

    p2 = p1 = buffer;
    while( *p1 )
    {
        /* convert CR/LF to LF */
        if( *p1 == '\r' && p1[ 1 ] == '\n')
            p1++;

        /* remove a last new line */
        if( *p1 == '\n' && !p1[ 1 ])
            break;

        *p2++ = *p1++;
    }
    *p2 = '\0';

    return buffer;
}

#include <stdio.h>

#define ERROR_CODE  3175

int main( void )
{
    char buf[ 512 ];

    printf("Error message for error code %d :\n%s\n",
           ERROR_CODE, DosGetErrorMessage( ERROR_CODE, buf, sizeof( buf )));

    return 0;
}
