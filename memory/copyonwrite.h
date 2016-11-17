/*
 * COPY-ON-WRITE emulation for OS/2
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file copyonwrite.h */

#ifndef OS2CODES_COPYONWRITE_H
#define OS2CODES_COPYONWRITE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Perform copy-on-write
 *
 * @param[in] p  Pointer to the source memory for copy-on-write.
 * @param[in] cb Bytes to copy-on-write.
 * @return Pointer to the destination memory for copy-on-write.
 * @todo Handle DosSetMem( PAG_DECOMMIT )
 */
void *copyOnWrite( const void *p, int cb );

#ifdef __cplusplus
}
#endif

#endif
