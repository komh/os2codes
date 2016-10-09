/*
 * DosAliasMemEx()
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_DOSALIASMEM_H
#define OS2CODES_DOSALIASMEM_H

#define INCL_DOS
#define INCL_EXAPIS
#include <os2.h>

/**
 * Extended DosAliasMem().
 *
 * @returns See DosAliasMem().
 * @param   pMem    Memory to be aliased.
 * @param   cbSize  Number of bytes to alias.
 * @param   ppAlias Where to store the address of the aliased memory.
 *                  If OBJ_LOCATION is specified in the flFlags, *ppAlias
 *                  will be the address of the object must be aliased at.
 * @param   flFlags Allocation flags. This API supports the same flags as
 *                  DosAliasMem() but adds OBJ_LOCATION.
 *                  If OBJ_LOCATION is specified, the object will be aliased
 *                  at the address specified by *ppAlias.
 */
APIRET DosAliasMemEx( PVOID pMem, ULONG cbSize, PPVOID ppAlias,
                      ULONG flFlags );

#endif
