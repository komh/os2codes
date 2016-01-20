/*
 * High-memory functions test program
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
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
#include <stdint.h>

int checkHighMem( void *p )
{
    intptr_t i = ( intptr_t )p;

    return i > 512L * 1024 * 1024;
}

int main( void )
{
    const char *s = "string";

    void *pm;
    void *pc;
    void *ps;

    pm = malloc( 1 );
    printf("malloc()  = %p, highmem = %d\n", pm, checkHighMem( pm ));

    pc = calloc( 1, 1 );
    printf("calloc()  = %p, highmem = %d\n", pc, checkHighMem( pc ));

    pc = realloc( pc , 1 );
    printf("realloc() = %p, highmem = %d\n", pc, checkHighMem( pc ));

    ps = strdup( s );
    printf("strdup()  = %p, highmem = %d\n", ps, checkHighMem( ps ));

    free( ps );
    free( pc );
    free( pm );

    return 0;
}
