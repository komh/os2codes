/*
 * DosGetErrorMessage(): get an error message from an error code
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file dosgeterrormessage.h */

#ifndef OS2CODES_DOSGETERRORMESSAGE_H
#define OS2CODES_DOSGETERRORMESSAGE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get an error message from an error code, @a errnum
 * @param[in] errnum Error code
 * @param[out] buffer Place to store an error message
 * @param[in] n Size of the buffer pointed by @a buffer in bytes
 * @return @a buffer on success, otherwise NULL
 */
const char *DosGetErrorMessage( int errnum, char *buffer, size_t n );

#ifdef __cplusplus
}
#endif

#endif
