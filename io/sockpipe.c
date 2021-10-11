/*
 * pipe() supporting a non-blocking mode using a socket for OS/2
 *
 * Copyright (C) 2014-2021 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <sys/socket.h>     /* for socketpair() */

int sockpipe( int *sv )
{
    int ret = socketpair( AF_LOCAL, SOCK_STREAM, 0, sv );

    if( ret == 0 )
    {
        /* prevent writing to a input handle */
        shutdown( sv[ 0 ], 1 );

        /* prevent reading from a output handle */
        shutdown( sv[ 1 ], 0 );
    }

    return ret;
}
