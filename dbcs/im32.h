/*
 * OS/2 32 bit Input Method dynamic loader
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_IM32_H
#define OS2CODES_IM32_H

#include <os2.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL im32Init( VOID );
VOID im32Term( VOID );
BOOL im32Inited( VOID );

#ifdef __cplusplus
}
#endif

#endif

