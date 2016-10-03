/*
 * functions to set a code page
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOS
#define INCL_DOSERRORS
#include <os2.h>

#include <stdlib.h>
#include <errno.h>

#include "setprocesscp.h"

#define WORKPLACE_PRIMARY_CP "WORKPLACE_PRIMARY_CP"

int setProcessCp( int cp, int inherit )
{
    if( inherit == CP_INHERIT && getenv( WORKPLACE_PRIMARY_CP )
        && putenv( WORKPLACE_PRIMARY_CP "=?") != 0 )
        return -1;

    if( DosSetProcessCp( cp ) != NO_ERROR )
    {
        errno = EINVAL;

        return -1;
    }

    return 0;
}
