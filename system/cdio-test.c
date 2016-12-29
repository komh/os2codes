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

static void showSessionInfo( int drive )
{
    char cmd[ 10 ] = {0, };

    struct {
        unsigned char len[ 2 ];
        unsigned char firstSession;
        unsigned char lastSession;

        /* last session info */
        unsigned char reserved1;
        unsigned char controlAndAdr;
        unsigned char firstTrack;
        unsigned char reserved2;
        unsigned char lba[ 4 ];
    } toc;

    int rc;


    HCDIO hcdio;

    hcdio = cdioOpen( drive );
    if( hcdio == ( HCDIO )-1 )
    {
        fprintf( stderr, "Could not get a handle of CD drive!!!\n");

        return;
    }

    cmd[ 0 ] = 0x43;    /* op. code for READ TOC/PMA/ATIP */
    cmd[ 2 ] = 0x01;    /* session info */
    cmd[ 7 ] = ( sizeof( toc ) >> 8 ) & 0xFF; /* MSG of allocation length */
    cmd[ 8 ] = sizeof( toc ) & 0xFF;          /* LSB of allocation length */

    rc = cdioExecScsiCmd( hcdio, CDIO_SCSI_READ,
                          cmd, 10 /* cmd length of 0x43 op. code */,
                          &toc, sizeof( toc ));

    if( rc == 0 )
    {
        printf("First session = %d\n", toc.firstSession );
        printf("Last session = %d\n", toc.lastSession );
        printf("First track of last session = %d\n", toc.firstTrack );
    }
    else
        fprintf( stderr, "cdioExecScsiCmd() failed!!!, rc = %d\n", rc );

    cdioClose( hcdio );
}

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

    printf("Press Enter to show session infomation of first CD drive\n");
    getchar();

    showSessionInfo( drive );

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
