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

#define INCL_DOS
#include <os2.h>

#include "getmodulename.h"

void *getModuleName( void *pSymbol, char *pszModName, size_t cbModName )
{
    HMODULE hmod;

    if( DosQueryModFromEIP( &hmod, NULL, 0, NULL, NULL, ( ULONG )pSymbol ))
        return NULL;

    if( cbModName && pszModName
        && DosQueryModuleName( hmod, cbModName, pszModName ))
        return NULL;

    return ( void * )hmod;
}
