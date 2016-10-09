/*
 * DosAliasMemEx()
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This file was modifed from src/emx/src/lib/sys/DosAllocMemEx.c of kLIBC.
 *
 * The following is the original license header.
 * -----
 *
 * DosAllocMemEx.
 *
 * Copyright (c) 2004 knut st. osmundsen <bird-srcspam@anduin.net>
 * Copyright (c) 2004 nickk
 *
 *
 * This file is part of InnoTek LIBC.
 *
 * InnoTek LIBC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * InnoTek LIBC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with InnoTek LIBC; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * -----
 */


#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_EXAPIS
#include <os2.h>

#include "dosaliasmemex.h"

#include <stdint.h>

/**
 * Alias memory at a given address.
 *
 * @remark  This algorithm is an improoved version of the one Odin uses in
 *          it's Ring-3 PeLdr.
 */
static int aliasAtAddress(void *pMem, ULONG cbReq, void *pvReq, ULONG fReq)
{
    PVOID           apvTmps[512]; /* 512MB */
    ULONG           cbTmp;
    ULONG           fTmp;
    int             iTmp;
    int             rcRet = ERROR_NOT_ENOUGH_MEMORY;

    /*
     * Adjust and size.
     */
    cbReq = (cbReq + 0xfff) & ~0xfff;

    /*
     * Allocation loop.
     * This algorithm is not optimal!
     */
    fTmp  = fPERM;
    cbTmp = 1*1024*1024; /* 1MB*/
    for (iTmp = 0; iTmp < sizeof(apvTmps) / sizeof(apvTmps[0]); iTmp++)
    {
        PVOID   pvNew = NULL;
        int     rc;

        /* Allocate chunk. */
        rc = DosAllocMem(&pvNew, cbTmp, fTmp);
        apvTmps[iTmp] = pvNew;
        if (rc)
            break;

        /*
         * Passed it?
         * Then retry with the requested size.
         */
        if (pvNew > pvReq)
        {
            if (cbTmp <= cbReq)
                break;
            DosFreeMem(pvNew);
            cbTmp = cbReq;
            iTmp--;
            continue;
        }

        /*
         * Does the allocated object touch into the requested one?
         */
        if ((char *)pvNew + cbTmp > (char *)pvReq)
        {
            /*
             * Yes, we've found the requested address!
             */
            apvTmps[iTmp] = NULL;
            DosFreeMem(pvNew);

            /*
             * Adjust the allocation size to fill the gap between the
             * one we just got and the requested one.
             * If no gap we'll attempt the real allocation.
             */
            cbTmp = (uintptr_t)pvReq - (uintptr_t)pvNew;
            if (cbTmp)
            {
                iTmp--;
                continue;
            }

            rc = DosAliasMem(pMem, cbReq, &pvNew, fReq);
            if (rc || (char *)pvNew > (char *)pvReq)
                break; /* we failed! */
            if (pvNew == pvReq)
            {
                rcRet = 0;
                break;
            }

            /*
             * We've got an object which start is below the one we
             * requested. This is probably caused by the requested object
             * fitting in somewhere our tmp objects didn't.
             * So, we'll have loop and retry till all such holes are filled.
             */
            apvTmps[iTmp] = pvNew;
        }
    }

    /*
     * Cleanup reserved memory and return.
     */
    while (iTmp-- > 0)
        if (apvTmps[iTmp])
            DosFreeMem(apvTmps[iTmp]);

    return rcRet;
}

/**
 * Extended DosAliasMem().
 *
 * @returns See DosAliasMem().
 * @param   pMem    Memory to be aliased.
 * @param   cbSize  Number of bytes to alias.
 * @param   ppAlias Where to store the address of the aliased memory.
 *                  If OBJ_LOCATION is specified in the flFlags, *ppAlias
 *                  will be the address of the object must be aliased at.
 * @param   flFlags Allocation flags. This API supports the same flags as
 *                  DosAliasMem() but adds OBJ_LOCATION.
 *                  If OBJ_LOCATION is specified, the object will be aliased
 *                  at the address specified by *ppAlias.
 */
APIRET DosAliasMemEx( PVOID pMem, ULONG cbSize, PPVOID ppAlias, ULONG flFlags )
{
    int     rc;

    /*
     * Validate input first.
     * DosAliasMem() cannot alias at a high memory(>512MB).
     */
    if (    flFlags & OBJ_LOCATION
        &&  (   (uintptr_t)*ppAlias < 0x10000
             || (uintptr_t)*ppAlias >= 0x20000000/*512MB*/)
        )
        return ERROR_INVALID_ADDRESS;

    /*
     * Make the allocation.
     */
    if (flFlags & OBJ_LOCATION)
        rc = aliasAtAddress(pMem, cbSize, *ppAlias, flFlags & ~OBJ_LOCATION);
    else
        rc = DosAliasMem(pMem, cbSize, ppAlias, flFlags);

    return rc;
}
