#include <stdio.h>
#include <string.h>

#include "fat32io.h"

#define SECTOR_SIZE 512

int main( int argc, char *argv[])
{
    HFAT32IO hfat32io;
    char     buf[ SECTOR_SIZE * 2 ];
    int      rc = 0;

    if( argc < 2 )
    {
        printf("Specify drive letter!!!\n");

        return 1;
    }

    memset( buf, 0, SECTOR_SIZE );
    memset( buf + SECTOR_SIZE, 0xFF, SECTOR_SIZE );

    hfat32io = fat32ioOpen( argv[ 1 ][ 0 ] - 'A');
    if( hfat32io == ( HFAT32IO)-1 )
    {
        fprintf( stderr, "fat32ioOpen(%s) failed!!!\n", argv[ 1 ]);

        rc = 1;
        goto exit_error;
    }

    if( fat32ioLock( hfat32io ))
        fprintf( stderr, "fat32ioLock() failed!!!\n");

    if( fat32ioInitSectorIO( hfat32io ))
    {
        fprintf( stderr, "fat32ioInitSectorIo() failed!!!\n");

        rc = 1;
        goto exit_unlock;
    }

    if( fat32ioReadSectors( hfat32io, 0, 1, buf ) == -1 )
    {
        fprintf( stderr, "fat32ioReadSectors() failed!!!\n");

        rc = 1;
        goto exit_unlock;
    }

    if( fat32ioWriteSectors( hfat32io, 0, 1, buf ) == -1 )
    {
        fprintf( stderr, "fat32ioWriteSectors() failed!!!\n");

        rc = 1;
        goto exit_unlock;
    }

    if( fat32ioReadSectors( hfat32io, 0, 1, buf + SECTOR_SIZE ) == -1 )
    {
        fprintf( stderr, "fat32ioReadSectors() failed!!!\n");

        rc = 1;
        goto exit_unlock;
    }

    if( memcmp( buf, buf + SECTOR_SIZE, SECTOR_SIZE ))
    {
        printf("fat32io read/write test failed!!!\n");

        rc = 1;
    }
    else
        printf("fat32io read/write test succeeded!!!\n");

exit_unlock:

    if( fat32ioUnlock( hfat32io ))
        fprintf( stderr, "fat32ioUnlock() failed!!!\n");

    if( fat32ioClose( hfat32io ))
    {
        fprintf( stderr, "fat32ioClose() failed!!!\n");

        rc = 1;
    }

exit_error:

    return rc;
}
