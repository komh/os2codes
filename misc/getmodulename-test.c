/*
 * getModuleName() test program
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>

#include "getmodulename.h"

int main( void )
{
    void *mod;
    char szName[ 512 ];

    mod = getModuleName( fopen, szName, sizeof( szName ));

    printf("%s: mod = %p, name = %s\n", "fopen", mod, szName );

    return 0;
}
