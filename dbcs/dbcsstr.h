/*
 * DBCS string functions
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file dbcsstr.h */

#ifndef OS2CODES_DBCSSTR_H
#define OS2CODES_DBCSSTR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Test if DBCS is enabled
 * @return Non-zero on enabled, otherwise 0 on disabled
 */
int dbcsIsEnabled( void );

/**
 * @brief Calculate a length of a string containing DBCS characters
 * @param[in] str A string containing DBCS characters
 * @return A length of a string pointed by @a str in characters
 */
int dbcsStrlen( const char *str );

/**
 * @brief Check if a chracter at @a pos of @a str is a DBCS lead byte
 * @param[in] str A string containing DBCS characters
 * @param[in] pos A index of @a str
 * @return Non-zero if DBCS lead byte, or 0 if SBCS
 */
int dbcsIsLeadByte( const char *str, int pos );

/**
 * @brief Check if a character at @a pos of @a str is a DBCS trail byte
 * @param[in] str A string containing DBCS characters
 * @param[in] pos A index of @a str
 * @return Non-zero if DBCS trail byte, otherwise 0
 */
int dbcsIsTrailByte( const char *str, int pos );

#ifdef __cplusplus
}
#endif

#endif
