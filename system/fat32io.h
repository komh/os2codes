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
