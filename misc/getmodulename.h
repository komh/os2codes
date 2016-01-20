/*
 * getModuleName(): retrive a module handle and a module name from a symbol
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file getmodulename.h */

#ifndef OS2CODES_GETMODULENAME_H
#define OS2CODES_GETMODULENAME_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get a module name from a symbol @a pSymbol
 * @param[in] pSymvol An address of a symbol to retrive a module name from
 * @param[out] pszModName Place to store a module name
 * @param[in] cbModName Size of the buffer pointed by @a pszModName
 * @return Module handle on success, otherwise NULL
 * @remark It's possible to cast return value to HMODULE
 */
void *getModuleName( void *pSymbol, char *pszModName, size_t cbModName );

#ifdef __cplusplus
}
#endif

#endif
