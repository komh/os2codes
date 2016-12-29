/*
 * checkIfs() test program
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include "checkifs.h"

#include <stdio.h>

int main( void )
{
    const char *fsdNames[] = {"HPFS", "JFS", "FAT32", "CDFS", "UDF",
                              "NDFS32", "TVFS", "RAMFS", "LAN", "NFS",
                              NULL };
    int i;

    for( i = 0; fsdNames[ i ]; i++ )
    {
        printf("%s: %s\n", fsdNames[ i ],
               checkIfs( fsdNames[ i ]) ? "LOADED" : "NOT loaded");
    }

    return 0;
}
