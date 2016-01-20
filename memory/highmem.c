/*
 * High-memory functions
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdlib.h>
#include <string.h>

#include <emx/umalloc.h>

void *malloc( size_t size )
{
    void *p = _hmalloc( size );

    if( !p )
        p = _lmalloc( size );

    return p;
}

void *calloc( size_t elements, size_t size )
{
    void *p = _hcalloc( elements, size );

    if( !p )
        p = _lcalloc( elements, size );

    return p;
}

void *realloc( void *mem, size_t size )
{
    void *p = _hrealloc( mem, size );

    if( !p )
        p = _lrealloc( mem, size );

    return p;
}

char *_std_strdup( const char *string );

char *strdup( const char *string )
{
    void *s = _hstrdup( string );

    if( !s )
        s = _std_strdup( string );

    return s;
}
