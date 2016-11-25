/*
 * gethwaddr(): retrive MAC address of a network interface
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include "gethwaddr.h"

#include <string.h>

#include <types.h>
#include <sys/socket.h> /* socket() */
#include <io.h>         /* close() */
#include <sys/ioctl.h>  /* ioctl(), SIOCGIFADDR */
#include <net/if.h>     /* struct ifconf, struct ifreq */
#include <net/if_dl.h>  /* struct sockaddr_dl */

int gethwaddr( const char *ifname, HWADDR hwaddr )
{
    struct ifconf ifconf;
    struct ifreq iflist[ IFMIB_ENTRIES ];
    int s;
    int count;
    int i;

    s = socket( PF_INET, SOCK_RAW, 0 );
    if( s == -1 )
        return -1;

    memset( iflist, 0, sizeof( iflist ));

    ifconf.ifc_len = sizeof( iflist );
    ifconf.ifc_req = iflist;
    if( ioctl( s, SIOCGIFCONF, &ifconf ) < 0 )
    {
        close( s );

        return -1;
    }

    count = ifconf.ifc_len / sizeof( struct ifreq );
    for( i = 0; i < count; i++ )
    {
        if( iflist[ i ].ifr_addr.sa_family == AF_LINK
            && strcmp( ifname, iflist[ i ].ifr_name ) == 0 )
        {
            struct sockaddr_dl *sdl =
                ( struct sockaddr_dl * )&iflist[ i ].ifr_addr;

            /* Copy MAC address */
            memcpy( hwaddr, LLADDR( sdl ), 6 );

            break;
        }
    }

    return i == count /* not found ? */ ? -1 : 0;
}

