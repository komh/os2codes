/*
 * CD I/O test program
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>

#include "cdio.h"

int main( int argc, char *argv[])
{
    int drive;
    int count;
    int i;

    if( cdioGetDriveLetters( &drive, &count ))
    {
        fprintf( stderr, "cdioGetDriveLetters() failed!!!\n");

        return 1;
    }

    printf("CD drives : ");
    for( i = 0; i < count; i++)
        printf("[%c:] ", drive + i + 'A');
    printf("\n");

    printf("Press Enter to open a tray\n");
    getchar();

    if( cdioOpenTray( drive ))
        fprintf( stderr, "cdioOpenTray() failed!!!\n");

    printf("Press Enter to close a tray\n");
    getchar();

    if( cdioCloseTray( drive ))
        fprintf( stderr, "cdioCloseTray() failed!!!\n");

    return 0;
}
