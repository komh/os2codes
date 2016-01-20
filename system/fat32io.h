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

/** @file fat32io.h */

#ifndef OS2CODES_FAT32IO_H
#define OS2CODES_FAT32IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** FAT32IO handle */
typedef void *HFAT32IO;

/**
 * @brief Get a FAT32IO handle from a drive @a drive
 * @param[in] drive FAT32 drive. 0 = A, 1 = B, 2 = C and so forth
 * @return FAT32IO handle on success, otherwise (HFAT32IO)-1
 */
HFAT32IO fat32ioOpen( int drive );

/**
 * @brief Close a FAT32IO handle
 * @param[in] hfat32io FAT32IO handle
 * @return 0(NO_ERROR) on success, otherwise OS/2 errors
 */
int fat32ioClose( HFAT32IO hfat32io );

/**
 * @brief Initialize sector based IO mode
 * @param[in] hfat32io FAT32IO handle
 * @return 0(NO_ERROR) on success, otherwise OS/2 errors
 */
int fat32ioInitSectorIO( HFAT32IO hfat32io );

/**
 * @brief Read sectors from FAT32 drive
 * @param[in] hfat32io FAT32IO handle
 * @param[in] iSector Sector to read from
 * @param[in] nSectors A number of sectors to read
 * @param[out] pSectors Place to store contents of sectors
 * @return A number of bytes read if non-sector IO mode on success
 * @return A number of sectors read if sector IO mode on success
 * @return -1 on error
 */
int fat32ioReadSectors( HFAT32IO hfat32io, uint32_t iSector, size_t nSectors,
                        void *pSectors );

/**
 * @brief Write a buffer to FAT32 sectors
 * @param[in] hfat32io FAT32IO handle
 * @param[in] iSector Sector to write to
 * @param[in] nSectors A number of sectors to write
 * @param[out] pSectors Place to hold contents of sectors
 * @return A number of bytes written if non-sector IO mode on success
 * @return A number of sectors written if sector IO mode on success
 * @return -1 on error
 */
int fat32ioWriteSectors( HFAT32IO hfat32io, uint32_t iSector, size_t nSectors,
                         void *pSectors );

/**
 * @brief Lock a FAT32 drive
 * @param[in] hfat32io FAT32IO handle
 * @return 0(NO_ERROR) on success, otherwise OS/2 errors
 */
int fat32ioLock( HFAT32IO hfat32io );

/**
 * @brief Unlock a FAT32IO handle
 * @param[in] hfat32io FAT32IO handle
 * @return 0(NO_ERROR) on success, otherwise OS/2 errors
 */
int fat32ioUnlock( HFAT32IO hfat32io );

#ifdef __cplusplus
}
#endif
#endif
