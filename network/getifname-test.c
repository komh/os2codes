/*
 * getifname() test program
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include "getifname.h"

#include <stdio.h>
#include <string.h>

#include <types.h>
#include <sys/socket.h>
#include <io.h>
#include <sys/ioctl.h>
#include <net/if.h>

int main( void )
{
    int s;
    struct ifmib ifmib;
    int i, j;
    char ifname[ IFNAMSIZ ];

    s = socket( PF_INET, SOCK_RAW, 0 );

    os2_ioctl( s, SIOSTATIF42, &ifmib, sizeof( ifmib ));

    for( i = 0, j = 0; i < IFMIB_ENTRIES && j < ifmib.ifNumber; i++ )
    {
        /* Skip empty(?) entries */
        if( ifmib.iftable[ i ].iftType == 0 )
            continue;

        getifname( ifname, ifmib.iftable[ i ].iftIndex,
                   ifmib.iftable[ i ].iftDescr ),

        printf("%d: ifname = [%s], index = %d, description = [%s]\n",
               i, ifname,
               ifmib.iftable[ i ].iftIndex, ifmib.iftable[ i ].iftDescr );

        j++;
    }

    close( s );

    return 0;
}
