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

/** @file cdio.h */

#ifndef OS2CODES_CDIO_H
#define OS2CODES_CDIO_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Handle for CDIO */
typedef void *HCDIO;

/**
 * @def cdioScsiModes CDIO SCSI modes for #cdioExecScsiCmd()
 * @{
 */
/** write mode */
#define CDIO_SCSI_WRITE    0x00
/** read mode */
#define CDIO_SCSI_READ     0x01

/** does not check if device is playing audio */
#define CDIO_SCSI_PLAYING_IGNORE 0x00
/** return error if device is playing audio */
#define CDIO_SCSI_PLAYING_CHECK  0x20
/** @} */

/**
 * @brief Unlock a CD-ROM drive
 * @param[in] drive CD-ROM drive. 0 = A, 1 = B, C = 2 and so forth
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 */
int cdioUnlockDrive( int drive );

/**
 * @brief Lock a CD-ROM drive
 * @param[in] drive CD-ROM drive. 0 = A, 1 = B, C = 2 and so forth
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 */
int cdioLockDrive( int drive );

/**
 * @brief Open a tray of a CD-ROM drive
 * @param[in] drive CD-ROM drive. 0 = A, 1 = B, C = 2 and so forth
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 */
int cdioOpenTray( int drive );

/**
 * @brief Close a tray of a CD-ROM drive
 * @param[in] drive CD-ROM drive. 0 = A, 1 = B, C = 2 and so forth
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 */
int cdioCloseTray( int drive );

/**
 * @brief Get a first CD-ROM drive letter and a number of CD-ROM drives
 * @param[out] drive First CD-ROM drive, 0 = A, 1 = B, C = 2 and so forth
 * @param[out] count A number of CD-ROM drives.
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 * @remark CD-ROM drive letters are continuous from the first drive letter.
 * For example, if the first CD-ROM drive letter is S and a number of CD-ROM
 * drives is 3, then CD-ROM drive letters are S, T, and U.
 */
int cdioGetDriveLetters( int *drive, int *count );

/**
 * @brief Get a CDIO handle for CD-ROM drive
 * @param[in] drive CD-ROM drive. 0 = A, 1 = B, C = 2 and so forth
 * @return CDIO handle on success, (HCDIO)-1 On error
 */
HCDIO cdioOpen( int drive );

/**
 * @brief Close a CDIO handle
 * @param[in] hcdio CDIO handle
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 */
int cdioClose( HCDIO hcdio );

/**
 * @brief Execute SCSI commands
 * @param[in] hcdio CDIO handle
 * @param[in] mode SCSI modes. See @ref cdioScsiModes
 * @param[in] pCmdBuf Command buffer
 * @param[in] cbCmdBuf Size of command buffer in bytes
 * @param[in,out] pDataBuf Data buffer
 * @param[in] cbDataBuf Size of dat buffer in bytes
 * @return 0(NO_ERROR) on success, OS/2 errors on error
 */
int cdioExecScsiCmd( HCDIO hcdio, int mode, void *pCmdBuf, size_t cbCmdBuf,
                     void *pDataBuf, size_t cbDataBuf );

#ifdef __cplusplus
}
#endif

#endif
