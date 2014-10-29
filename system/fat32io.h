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

#ifndef OS2CODES_FAT32IO_H
#define OS2CODES_FAT32IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *HFAT32IO;

HFAT32IO fat32ioOpen( int drive );
int      fat32ioClose( HFAT32IO hfat32io );
int      fat32ioInitSectorIO( HFAT32IO hfat32io );
int      fat32ioReadSectors( HFAT32IO hfat32io, uint32_t iSector,
                             size_t nSectors, void *pSectors );
int      fat32ioWriteSectors( HFAT32IO hfat32io, uint32_t iSector,
                              size_t nSectors, void *pSectors );
int      fat32ioLock( HFAT32IO hfat32io );
int      fat32ioUnlock( HFAT32IO hfat32io );

#ifdef __cplusplus
}
#endif
#endif
