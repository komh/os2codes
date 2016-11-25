/*
 * getifname(): mangle an interface name from an index and a description
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <string.h>

char *getifname( char *ifname, int index, const char *descr )
{
    /*
     * The following codes are modified from
     * http://trac.netlabs.org/odin32/browser/trunk/src/iphlpapi/iphlpapi.cpp#L169.
     */

    if( index >= 0 && index < 9 )       /* lanX */
    {
        strcpy( ifname, "lan");
        ifname[ 3 ] = index + '0';
        ifname[ 4 ] = 0;
    }
    else if( strstr( descr, "back"))    /* loopback */
        strcpy( ifname, "lo");
    else if( strstr( descr, "ace ppp")) /* pppX */
        strcpy( ifname, strstr( descr, "ppp"));
    else if( strstr( descr, "ace sl"))   /* slX */
        strcpy( ifname, strstr( descr, "sl"));
    else if( strstr( descr, "ace dod"))  /* dodX */
        strcpy( ifname, strstr( descr, "dod"));
    else    /* something else... */
    {
        strcpy( ifname, "unk");
        ifname[ 3 ] = index + '0';
        ifname[ 4 ] = 0;
    }

    return ifname;
}

