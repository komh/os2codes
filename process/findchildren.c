/*
 * findchildren(): Find PIDs of children
 *
 * Copyright (C) 2024 KO Myung-Hun <komh78@gmail.com>
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

#include "findchildren.h"

#define BUF_SIZE_DELTA  ( 32 * 1024 )

int findchildren( pid_t pid, pid_t children[], size_t maxchildren )
{
    char *buf = NULL;
    int bufsize = 0;
    int nchildren;
    ULONG rc;

    do
    {
        free( buf );
        bufsize += BUF_SIZE_DELTA;
        buf = malloc( bufsize );
        if (buf == NULL)
        {
            errno = ENOMEM;

            return -1;
        }

        rc = DosQuerySysState( QS_PROCESS, 0, 0, 0, buf, bufsize );
    } while( rc == ERROR_BUFFER_OVERFLOW );

    if( rc == 0 )
    {
        QSPREC *pproc = (( QSPTRREC * )buf )->pProcRec;

        nchildren = 0;
        while( pproc && pproc->RecType == 1 )
        {
            if( pproc->ppid == pid )
            {
                if( nchildren < maxchildren )
                    children[ nchildren ] = pproc->pid;

                nchildren++;
            }

            pproc = (QSPREC *)(pproc->pThrdRec + pproc->cTCB);
        }
    }
    else
        nchildren = -1;

    free(buf);

    return nchildren;
}
