/*
 * CD I/O functions
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
#define INCL_DOSERRORS
#include <os2.h>

#include <string.h>

#include "cdio.h"

#define CDIO_UNLOCK_DRIVE   0
#define CDIO_LOCK_DRIVE     1
#define CDIO_EJECT_MEDIA    2
#define CDIO_LOAD_MEDIA     3

static int dskUnlockEjectMedia( int drive, int cmd )
{
#pragma pack( 1 )
    struct {
        BYTE bCmdInfo;
        BYTE bDrive;
    } param;
#pragma pack()

    ULONG ulParamLen = sizeof( param );
    ULONG ulDataLen  = 0;

    param.bCmdInfo = cmd;
    param.bDrive   = drive;

    return DosDevIOCtl(( HFILE )-1, IOCTL_DISK, DSK_UNLOCKEJECTMEDIA,
                       &param, ulParamLen, &ulParamLen,
                       NULL, ulDataLen, &ulDataLen );
}

int cdioUnlockDrive( int drive )
{
    return dskUnlockEjectMedia( drive, CDIO_UNLOCK_DRIVE );
}

int cdioLockDrive( int drive )
{
    return dskUnlockEjectMedia( drive, CDIO_LOCK_DRIVE );
}

int cdioOpenTray( int drive )
{
    return dskUnlockEjectMedia( drive, CDIO_EJECT_MEDIA );
}

int cdioCloseTray( int drive )
{
    return dskUnlockEjectMedia( drive, CDIO_LOAD_MEDIA );
}

#define IOCTL_CDROMDISK2        0x82
#define CDROMDISK2_DRIVELETTERS 0x60

int cdioGetDriveLetters( int *drive, int *count )
{
    HFILE hcd2;
    ULONG ulAction;
    ULONG ulParamLen = 0;
#pragma pack( 1 )
    struct
    {
        USHORT usDriveCount;
        USHORT usDriveFirst;
    } data;
#pragma pack()
    ULONG ulDataLen = sizeof( data );

    ULONG rc;

    rc = DosOpen(( PSZ )"CD-ROM2$", ( PHFILE )&hcd2, &ulAction, 0, FILE_NORMAL,
                  OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
                  OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                  NULL );

    if( rc )
        return rc;

    rc = DosDevIOCtl( hcd2, IOCTL_CDROMDISK2, CDROMDISK2_DRIVELETTERS,
                      NULL, ulParamLen, &ulParamLen,
                      &data, ulDataLen, &ulDataLen );

    if( !rc )
    {
        *drive = data.usDriveFirst;
        *count = data.usDriveCount;
    }

    DosClose( hcd2 );

    return rc;
}


HCDIO cdioOpen( int drive )
{
    HFILE hfile;
    char  szName[] = "X:";
    ULONG ulAction;
    ULONG rc;

    szName[ 0 ] = drive + 'A';

    rc = DosOpen( szName, &hfile, &ulAction, 0, FILE_NORMAL,
                  OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
                  OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE | OPEN_FLAGS_DASD,
                  NULL );

    return rc ? ( HCDIO )-1 : ( HCDIO )hfile;
}

int cdioClose( HCDIO hcdio )
{
    return DosClose(( HFILE )hcdio );
}

#define CDROMDISK_EXECMD    0x7A

/* 0, if transfer data to device,  1, if transfer data from device */
#define EX_DIRECTION_IN     0x0001
/* 0, if don't check playing audio, 1, if device plays audio return error */
#define EX_PLAYING_CHK      0x0002

int cdioExecScsiCmd( HCDIO hcdio, int mode, void *pCmdBuf, size_t cbCmdBuf,
                     void *pDataBuf, size_t cbDataBuf )
{
#pragma pack( 1 )
    struct {
        UCHAR   auchSign[ 4 ];      // 'CD01'
        USHORT  usDataLength;       // length of the Data Packet
        USHORT  usCmdLength;        // length of the Command Buffer
        USHORT  usFlags;            // flags
        BYTE    abCmdBuffer[ 16 ];  // Command Buffer for SCSI command
    } param = {
        .auchSign = {'C', 'D', '0', '1'},
    };
#pragma pack()

    ULONG ulParamLen = sizeof( param );
    ULONG ulDataLen = cbDataBuf;
    ULONG rc;

    if( cbCmdBuf > sizeof( param.abCmdBuffer ))
        return ERROR_INVALID_PARAMETER;

    param.usDataLength = ulDataLen;
    param.usCmdLength = cbCmdBuf;
    param.usFlags = (( mode & CDIO_SCSI_READ ) ? EX_DIRECTION_IN : 0 ) |
                    (( mode & CDIO_SCSI_PLAYING_CHECK ) ? EX_PLAYING_CHK : 0 );

    memcpy( param.abCmdBuffer, pCmdBuf, cbCmdBuf );

    rc = DosDevIOCtl(( HFILE )hcdio, IOCTL_CDROMDISK, CDROMDISK_EXECMD,
                     &param, ulParamLen, &ulParamLen,
                     pDataBuf, ulDataLen, &ulDataLen );

    return rc;
}
