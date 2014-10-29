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

#ifndef OS2CODES_CDIO_H
#define OS2CODES_CDIO_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *HCDIO;

/* write mode */
#define CDIO_SCSI_WRITE    0x00
/* read mode */
#define CDIO_SCSI_READ     0x01

/* does not check if device is playing audio */
#define CDIO_SCSI_PLAYING_IGNORE 0x00
/* return error if device is playing audio */
#define CDIO_SCSI_PLAYING_CHECK  0x20

int cdioUnlockDrive( int drive );
int cdioLockDrive( int drive );
int cdioOpenTray( int drive );
int cdioCloseTray( int drive );
int cdioGetDriveLetters( int *drive, int *count );

HCDIO cdioOpen( int drive );
int   cdioClose( HCDIO hcdio );
int   cdioExecScsiCmd( HCDIO hcdio, int mode, void *pCmdBuf, size_t cbCmdBuf,
                       void *pDataBuf, size_t cbDataBuf );

#ifdef __cplusplus
}
#endif

#endif
