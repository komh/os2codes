/*
 * getFreePhysMem(): Get size of free physical memory
 *
 * Copyright (C) 2021 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOS
#include <os2.h>

#include "getfreephysmem.h"

USHORT _THUNK_FUNCTION( Dos16MemAvail )( PULONG );

uint32_t getFreePhysMem( void )
{
    ULONG ulFreeMem;

    return (( USHORT )
            ( _THUNK_PROLOG( 4 );
              _THUNK_FLAT( &ulFreeMem );
              _THUNK_CALL( Dos16MemAvail ))) == 0 ? ulFreeMem : 0;
}
