/*
 * FIFO test program
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
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fifo.h"

#define ARRAY_SIZE( a ) ( sizeof(( a )) / sizeof(( a )[ 0 ]))

static int test1( void )
{
    HFIFO fifo;
    int in[] = { 1, 2, 3, 4, 5, 6, 7 };
    int out[ ARRAY_SIZE( in )];
    int i;

    printf("Testing NON-BLOCKING and FIXED fifo...\n");

    fifo = fifoCreate( ARRAY_SIZE( in ) / 2, sizeof( int ), 0 );

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoWrite() = %d\n", fifoWrite( fifo, in, ARRAY_SIZE( in )));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoRead() = %d\n", fifoRead( fifo, out, ARRAY_SIZE( out )));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    for( i = 0; i < ARRAY_SIZE( in ); i++ )
        printf("out[%d] = %d, in[%d] = %d\n", i, out[ i ], i, in[ i ]);

    fifoDestroy( fifo );

    printf("\n");

    return 0;
}

static int test2( void )
{
    HFIFO fifo;
    int in[] = { 1, 2, 3, 4, 5, 6, 7 };
    int out[ ARRAY_SIZE( in )];
    int i;

    printf("Testing NON-BLOCKING and EXPANDABLE fifo #1...\n");

    fifo = fifoCreate( ARRAY_SIZE( in ) / 2, sizeof( int ), FIFO_EXPANDABLE );

    printf("Elements = %d, Spaces = %d, Capacity = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ));

    printf("fifoWrite() = %d\n", fifoWrite( fifo, in, ARRAY_SIZE( in )));

    printf("Elements = %d, Spaces = %d, Capacity = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ));

    printf("fifoRead() = %d\n", fifoRead( fifo, out, ARRAY_SIZE( out )));

    printf("Elements = %d, Spaces = %d, Capacity = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ));

    for( i = 0; i < ARRAY_SIZE( in ); i++ )
        printf("out[%d] = %d, in[%d] = %d\n", i, out[ i ], i, in[ i ]);

    fifoDestroy( fifo );

    printf("\n");

    return 0;
}

static int test3( void )
{
    HFIFO fifo;
    int in[] = { 1, 2, 3, 4, 5, 6, 7 };
    int out[ ARRAY_SIZE( in )];
    int i;

    printf("Testing NON-BLOCKING and EXPANDABLE fifo #2...\n");

    fifo = fifoCreate( ARRAY_SIZE( in ) / 2, sizeof( int ), FIFO_EXPANDABLE );

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoWrite() = %d\n", fifoWrite( fifo, in, ARRAY_SIZE( in )));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoShrink() = %d\n", fifoShrink( fifo ));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoRead() = %d\n", fifoRead( fifo, out, ARRAY_SIZE( out )));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    for( i = 0; i < ARRAY_SIZE( in ); i++ )
        printf("out[%d] = %d, in[%d] = %d\n", i, out[ i ], i, in[ i ]);

    memset( out, 0xFF, sizeof( out ));

    printf("fifoShrink() = %d\n", fifoShrink( fifo ));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoWrite() = %d\n", fifoWrite( fifo, in, ARRAY_SIZE( in )));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    printf("fifoRead() = %d\n", fifoRead( fifo, out, ARRAY_SIZE( out )));

    printf("Elements = %d, Spaces = %d, Capacity = %d, Empty = %d, Full = %d\n",
           fifoElements( fifo ), fifoSpaces( fifo ), fifoCapacity( fifo ),
           fifoEmpty( fifo ), fifoFull( fifo ));

    for( i = 0; i < ARRAY_SIZE( in ); i++ )
        printf("out[%d] = %d, in[%d] = %d\n", i, out[ i ], i, in[ i ]);

    fifoDestroy( fifo );

    printf("\n");

    return 0;
}

struct threadarg
{
    HFIFO fifo;
    size_t arraysize;
    int *array;
};

static void reader( void *arg )
{
    struct threadarg *ta  = arg;
    int i;

    for( i = 0; i < ta->arraysize * 2; i++ )
    {
        printf("reader: fifoRead() = %d\n",
               fifoRead( ta->fifo, ta->array + ( i % ta->arraysize ), 1 ));
    }
}

static void writer( void *arg )
{
    struct threadarg *ta  = arg;
    int i;

    for( i = 0; i < ta->arraysize * 2; i++ )
    {
        printf("writer: fifoWrite() = %d\n",
               fifoWrite( ta->fifo, ta->array + ( i % ta->arraysize ), 1 ));
    }
}

static int test4( void )
{
    HFIFO fifo;
    int in[] = { 1, 2, 3, 4, 5, 6, 7 };
    int out[ ARRAY_SIZE( in )];
    struct threadarg readerarg;
    struct threadarg writerarg;
    TID readerid;
    TID writerid;
    int i;

    printf("Testing BLOCKING and FIXED fifo...\n");

    fifo = fifoCreate( ARRAY_SIZE( in ) / 2, sizeof( int ), FIFO_BLOCKING );

    readerarg.fifo = fifo;
    readerarg.arraysize = ARRAY_SIZE( out );
    readerarg.array = out;
    readerid = _beginthread( reader, NULL, 32768, &readerarg );

    writerarg.fifo = fifo;
    writerarg.arraysize = ARRAY_SIZE( in );
    writerarg.array = in;
    writerid = _beginthread( writer, NULL, 32768, &writerarg );

    DosWaitThread( &readerid, DCWW_WAIT );
    DosWaitThread( &writerid, DCWW_WAIT );

    for( i = 0; i < ARRAY_SIZE( in ); i++ )
        printf("out[%d] = %d, in[%d] = %d\n", i, out[ i ], i, in[ i ]);

    fifoDestroy( fifo );

    printf("\n");

    return 0;
}

static int test5( void )
{
    HFIFO fifo;
    int in[] = { 1, 2, 3, 4, 5, 6, 7 };
    int out[ ARRAY_SIZE( in )];
    struct threadarg readerarg;
    struct threadarg writerarg;
    TID readerid;
    TID writerid;
    int i;

    printf("Testing BLOCKING and EXPANDABLE fifo...\n");

    fifo = fifoCreate( ARRAY_SIZE( in ) / 2, sizeof( int ),
                       FIFO_BLOCKING | FIFO_EXPANDABLE );

    readerarg.fifo = fifo;
    readerarg.arraysize = ARRAY_SIZE( out );
    readerarg.array = out;
    readerid = _beginthread( reader, NULL, 32768, &readerarg );

    writerarg.fifo = fifo;
    writerarg.arraysize = ARRAY_SIZE( in );
    writerarg.array = in;
    writerid = _beginthread( writer, NULL, 32768, &writerarg );

    DosWaitThread( &readerid, DCWW_WAIT );
    DosWaitThread( &writerid, DCWW_WAIT );

    for( i = 0; i < ARRAY_SIZE( in ); i++ )
        printf("out[%d] = %d, in[%d] = %d\n", i, out[ i ], i, in[ i ]);

    fifoDestroy( fifo );

    printf("\n");

    return 0;
}

int main( void )
{
    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;
}
