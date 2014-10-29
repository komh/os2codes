/*
 * FAT32 I/O functions
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
#define INCL_DOSDEVIOCTL
#include <os2.h>

#include <stdio.h>
#include <string.h>

#include "fat32io.h"

#define SECTOR_SIZE         512

#define FAT32_SECTORIO      0x9014

#define IOCTL_FAT32         IOCTL_GENERAL
#define FAT32_READSECTOR    0xFD
#define FAT32_WRITESECTOR   0xFF

HFAT32IO fat32ioOpen( int drive )
{
    char szDrive[] = "X:";
    HFILE hfile;
    ULONG ulAction;
    ULONG rc;

    szDrive[ 0 ] = drive + 'A';

    rc = DosOpen( szDrive, &hfile, &ulAction, 0L, FILE_NORMAL,
                  OPEN_ACTION_OPEN_IF_EXISTS,
                  OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE
                  | OPEN_FLAGS_DASD,
                  NULL );

    return rc ? ( HFAT32IO)-1 : ( HFAT32IO)hfile;
}

int fat32ioClose( HFAT32IO hfat32io )
{
    return DosClose(( HFILE )hfat32io );
}

int fat32ioInitSectorIO( HFAT32IO hfat32io )
{
    ULONG cbData     = 0;
    ULONG ulDeadFace = 0xDEADFACE;
    ULONG cbParam    = sizeof( ulDeadFace );

    return DosFSCtl( NULL, cbData, &cbData,
                     &ulDeadFace, cbParam, &cbParam,
                     FAT32_SECTORIO, NULL, ( HFILE )hfat32io, FSCTL_HANDLE );
}

#define USE_DEVIOCTL

int fat32ioReadSectors( HFAT32IO hfat32io, uint32_t iSector,
                        size_t nSectors, void *pSectors )
{
#ifdef USE_DEVIOCTL
#pragma pack( 1 )
    struct
    {
        ULONG  ulSector;
        USHORT nSectors;
    } rsd;
#pragma pack()

    ULONG cbParam;
    ULONG cbData;

    rsd.ulSector = iSector;
    rsd.nSectors = nSectors;

    cbParam = sizeof( rsd );
    cbData  = rsd.nSectors * SECTOR_SIZE;

    if( DosDevIOCtl(( HFILE )hfat32io, IOCTL_FAT32, FAT32_READSECTOR,
                    &rsd, cbParam, &cbParam,
                    pSectors, cbData, &cbData ))
        return -1;

    return rsd.nSectors;
#else
    ULONG ulActual;

    if( DosSetFilePtr(( HFILE )hfat32io, iSector, FILE_BEGIN, &ulActual ))
        return -1;

    if( DosRead(( HFILE )hfat32io, pSectors, nSectors, &ulActual ))
        return -1;

    return ulActual;
#endif
}

int fat32ioWriteSectors( HFAT32IO hfat32io, uint32_t iSector,
                         size_t nSectors, void *pSectors )
{
#ifdef USE_DEVIOCTL
#pragma pack( 1 )
    struct
    {
        ULONG  ulSector;
        USHORT nSectors;
    } wsd;
#pragma pack()

    ULONG cbParam;
    ULONG cbData;

    wsd.ulSector = iSector;
    wsd.nSectors = nSectors;

    cbParam = sizeof( wsd );
    cbData  = wsd.nSectors * SECTOR_SIZE;

    if ( DosDevIOCtl(( HFILE )hfat32io, IOCTL_FAT32, FAT32_READSECTOR,
                     &wsd, cbParam, &cbParam,
                     pSectors, cbData, &cbData ))
        return -1;

    return wsd.nSectors;
#else
    ULONG ulActual;

    if( DosSetFilePtr(( HFILE )hfat32io, iSector, FILE_BEGIN, &ulActual ))
        return -1;

    if( DosWrite(( HFILE )hfat32io, pSectors, nSectors, &ulActual ))
        return -1;

    return ulActual;
#endif
}

int fat32ioLock( HFAT32IO hfat32io )
{
    BYTE bParam;
    BYTE bData;

    ULONG ulParamLen = sizeof( bParam );
    ULONG ulDataLen  = sizeof( bData );;

    bParam = 0;
    bData  = 0;

    return DosDevIOCtl(( HFILE )hfat32io, IOCTL_DISK, DSK_LOCKDRIVE,
                        &bParam, ulParamLen, &ulParamLen,
                        &bData, ulDataLen, &ulDataLen );
}

int fat32ioUnlock( HFAT32IO hfat32io )
{
    BYTE bParam;
    BYTE bData;

    ULONG ulParamLen = sizeof( bParam );
    ULONG ulDataLen  = sizeof( bData );

    bParam = 0;
    bData  = 0;

    return DosDevIOCtl(( HFILE )hfat32io, IOCTL_DISK, DSK_UNLOCKDRIVE,
                        &bParam, ulParamLen, &ulParamLen,
                        &bData, ulDataLen, &ulDataLen );
}

