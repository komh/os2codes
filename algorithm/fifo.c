/*
 * FIFO functions
 *
 * Copyright (C) 2024 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOS
#define INCL_DOSERRORS
#include <os2.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "fifo.h"

struct fifo
{
    char *buffer;
    size_t capacity;
    size_t elements;
    size_t readpos;
    size_t writepos;
    size_t elementsize;
    int blocking;
    int expandable;

    HMTX lock;
    HEV readsig;
    HEV writesig;
};

#define FIFO_LOCK( f )                                                      \
    while( DosRequestMutexSem(( f )->lock, SEM_INDEFINITE_WAIT )            \
                == ERROR_INTERRUPT )                                        \
        /* nothing */

#define FIFO_UNLOCK( f ) DosReleaseMutexSem(( f )->lock )

#define FIFO_SEM_POST( s ) DosPostEventSem(( s ))

#define FIFO_SEM_WAIT( s )                                                  \
    do {                                                                    \
        ULONG rc;                                                           \
        while(( rc = DosWaitEventSem(( s ), SEM_INDEFINITE_WAIT ))          \
                    == ERROR_INTERRUPT )                                    \
            /* nothing */;                                                  \
        if( rc == 0 )                                                       \
            DosResetEventSem(( s ), &rc );                                  \
    } while( 0 )

HFIFO fifoCreate( size_t capacity, size_t elementsize, int flags )
{
    HFIFO fifo;

    if( elementsize == 0
        || ( flags & ~( FIFO_BLOCKING | FIFO_EXPANDABLE )) != 0 )
    {
        errno = EINVAL;

        return NULL;
    }

    fifo = calloc( 1, sizeof( *fifo ));
    if( fifo == NULL )
    {
        errno = ENOMEM;

        return NULL;
    }

    fifo->buffer = malloc( capacity * elementsize );
    if( fifo->buffer == NULL )
    {
        free( fifo );

        errno = ENOMEM;

        return NULL;
    }

    fifo->capacity = capacity;
    fifo->elementsize = elementsize;
    fifo->blocking = flags & FIFO_BLOCKING;
    fifo->expandable = flags & FIFO_EXPANDABLE;

    if( DosCreateMutexSem( NULL, &fifo->lock, 0, FALSE ) != 0
        || DosCreateEventSem( NULL, &fifo->readsig, 0, FALSE ) != 0
        || DosCreateEventSem( NULL, &fifo->writesig, 0, FALSE ))
    {
        fifoDestroy( fifo );

        errno = ENOMEM;

        return NULL;
    }

    return fifo;
}

int fifoDestroy( HFIFO fifo )
{
    DosCloseEventSem( fifo->writesig );
    DosCloseEventSem( fifo->readsig );
    DosCloseMutexSem( fifo->lock );

    free( fifo->buffer );
    free( fifo );

    return 0;
}

static int fifoReadInternal( HFIFO fifo, void *buffer, size_t elements )
{
    char *buf = buffer;
    size_t remelements = 0;

    if( elements > fifo->elements )
        elements = fifo->elements;

    if( elements == 0 )
        return 0;

    if( fifo->readpos + elements > fifo->capacity )
    {
        remelements = fifo->readpos + elements - fifo->capacity;
        elements = fifo->capacity - fifo->readpos;
    }

    memcpy( buf, fifo->buffer + fifo->readpos * fifo->elementsize,
            elements * fifo->elementsize );

    if( remelements > 0 )
        memcpy( buf + elements * fifo->elementsize, fifo->buffer,
                remelements * fifo->elementsize );

    elements += remelements;
    fifo->elements -= elements;
    fifo->readpos = ( fifo->readpos + elements ) % fifo->capacity;

    return elements;
}

int fifoRead( HFIFO fifo, void *buffer, size_t elements )
{
    size_t elms = 0;

    FIFO_LOCK( fifo );

    for(;;)
    {
        elms += fifoReadInternal( fifo, buffer, elements - elms );
        FIFO_SEM_POST( fifo->writesig );

        if( fifo->blocking && elms < elements )
        {
            FIFO_UNLOCK( fifo );
            FIFO_SEM_WAIT( fifo->readsig );
            FIFO_LOCK( fifo );
        }
        else
            break;
    }

    FIFO_UNLOCK( fifo );

    return elms;
}

static char *fifoReallocBuffer( HFIFO fifo, size_t newcapacity )
{
    char *newbuffer;
    size_t savedelements;

    newbuffer = malloc( newcapacity * fifo->elementsize );
    if( newbuffer == NULL )
    {
        errno = ENOMEM;

        return NULL;
    }

    savedelements = fifo->elements;
    fifoReadInternal( fifo, newbuffer, fifo->elements );

    free( fifo->buffer );
    fifo->buffer = newbuffer;
    fifo->capacity = newcapacity;
    fifo->elements = savedelements;
    fifo->readpos = 0;
    fifo->writepos = fifo->elements;

    return newbuffer;
}

static int fifoWriteInternal( HFIFO fifo, const void *buffer, size_t elements )
{
    const char *buf = buffer;
    int remelements = 0;

    if( fifo->elements + elements > fifo->capacity )
        elements = fifo->capacity - fifo->elements;

    if( elements == 0 )
        return 0;

    if( fifo->writepos + elements > fifo->capacity )
    {
        remelements = fifo->writepos + elements - fifo->capacity;
        elements = fifo->capacity - fifo->writepos;
    }

    memcpy( fifo->buffer + fifo->writepos * fifo->elementsize, buf,
            elements * fifo->elementsize );

    if( remelements > 0 )
        memcpy( fifo->buffer, buf + elements * fifo->elementsize,
                remelements * fifo->elementsize );

    elements += remelements;
    fifo->elements += elements;
    fifo->writepos = ( fifo->writepos + elements ) % fifo->capacity;

    return elements;
}

int fifoWrite( HFIFO fifo, const void *buffer, size_t elements )
{
    size_t elms = 0;

    FIFO_LOCK( fifo );

    if( fifo->expandable && fifo->elements + elements > fifo->capacity )
    {
        size_t newcapacity = fifo->capacity << 1;
        size_t newelements = elements + fifo->elements;

        if( newcapacity == 0 )
            newcapacity = 1;

        while( newcapacity < newelements )
            newcapacity <<= 1;

        if( fifoReallocBuffer( fifo, newcapacity ) == NULL )
        {
            FIFO_UNLOCK( fifo );

            return -1;
        }
    }

    for(;;)
    {
        elms += fifoWriteInternal( fifo, buffer, elements - elms );
        FIFO_SEM_POST( fifo->readsig );

        if( fifo->blocking && elms < elements )
        {
            FIFO_UNLOCK( fifo );
            FIFO_SEM_WAIT( fifo->writesig );
            FIFO_LOCK( fifo );
        }
        else
            break;
    }

    FIFO_UNLOCK( fifo );

    return elms;
}

int fifoClear( HFIFO fifo )
{
    FIFO_LOCK( fifo );

    fifo->elements = 0;
    fifo->readpos = 0;
    fifo->writepos = 0;

    FIFO_UNLOCK( fifo );

    return 0;
}

int fifoShrink( HFIFO fifo )
{
    size_t capacity;

    FIFO_LOCK( fifo );

    if( fifo->elements == fifo->capacity )
    {
        FIFO_UNLOCK( fifo );

        return 0;
    }

    if( fifoReallocBuffer( fifo, fifo->elements ) == NULL )
    {
        FIFO_UNLOCK( fifo );

        return -1;
    }

    capacity = fifo->capacity;

    FIFO_UNLOCK( fifo );

    return capacity;
}

size_t fifoCapacity( const HFIFO fifo )
{
    size_t capacity;

    FIFO_LOCK( fifo );

    capacity = fifo->capacity;

    FIFO_UNLOCK( fifo );

    return capacity;
}

size_t fifoElements( const HFIFO fifo )
{
    size_t elements;

    FIFO_LOCK( fifo );

    elements = fifo->elements;

    FIFO_UNLOCK( fifo );

    return elements;
}

size_t fifoSpaces( const HFIFO fifo )
{
    size_t spaces;

    FIFO_LOCK( fifo );

    spaces = fifo->capacity - fifo->elements;

    FIFO_UNLOCK( fifo );

    return spaces;
}
