/*
 * COPY-ON-WRITE emulation for OS/2
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file copyonwrite.c */

#define INCL_DOS
#define INCL_EXAPIS
#include <os2.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "copyonwrite.h"

/**
 * Internal structure for a memory list
 */
typedef struct _MEMORYLIST
{
    void *pDest;        /**< Destination address of copy-on-write. */
    int fl;             /**< Permission flag of destination memory */
    int cb;             /**< Bytes. */
    const void *pSrc;   /**< Source address of copy-on-write. */

    struct _MEMORYLIST *pmlNext; /**< Pointer to the next memory list. */
} MEMORYLIST, *PMEMORYLIST;

/** Pointer to the start of memory list. */
static PMEMORYLIST pmlStart = NULL;

/**
 * Reallocate destination memory
 *
 * @param[in] pStart Start address of changed memory.
 * @param[in] pEnd   End address of changed memory.
 * @param[in] flDest true if finding destination memory
 *                   false if finding source memory.
 * @return true on success, false on error.
 */
static bool reallocDestMem( const void *pStart, const void *pEnd, bool flDest )
{
    const void *pAddrStart;
    const void *pAddrEnd;
    bool flProcessed = false;

    PMEMORYLIST pml;
    PMEMORYLIST *pmlNext = &pmlStart;

    /* Find releated memory list and perform copy */
    for( pml = pmlStart; pml; pml = *pmlNext )
    {
        pAddrStart = flDest ? pml->pDest : pml->pSrc;
        pAddrEnd = ( char * )pAddrStart + pml->cb;

        /* Changed memory is in maintained memory list ? */
        if( pAddrStart < pEnd && pStart < pAddrEnd)
        {
            flProcessed = false;

            /* Destination memory ? */
            if( pml->pSrc )
            {
                /* Yes, destination memory */

                /*
                 * Reallocate memory at current address and copy from source
                 * memory
                 */
                if( DosFreeMem( pml->pDest ))
                    break;

                if( DosAllocMemEx( &pml->pDest, pml->cb,
                                   fPERM | PAG_COMMIT | OBJ_LOCATION ))
                    break;

                memcpy( pml->pDest, pml->pSrc, pml->cb );
            }
            else
            {
                /* No, source memory */

                /* Just restore permission flag */
                if( DosSetMem( pml->pDest, pml->cb, pml->fl ))
                    break;
            }

            /* Remove current memory list */
            *pmlNext = pml->pmlNext;

            pAddrStart = pml->pDest;
            pAddrEnd = (char * )pAddrStart + pml->cb;

            free( pml );

            /*
             * Reallocate affectedd destination memory by changed memory,
             * recursively.
             */
            if( !reallocDestMem( pAddrStart, pAddrEnd, false ))
                break;

            flProcessed = true;
        }
        else
            pmlNext = &pml->pmlNext;
    }

    /*
     * Also OK even if not processed, if iterated all memory list to find
     * affected destination memory by changed memory.
     */
    return flProcessed || ( !pml && !flDest );
}

/**
 * Exception handler for SIGSEGV
 */
static ULONG _System sigsegv( PEXCEPTIONREPORTRECORD p1,
                              PEXCEPTIONREGISTRATIONRECORD p2,
                              PCONTEXTRECORD p3,
                              PVOID pv )
{
    /* Check if WRITE access violation */
    if( p1->ExceptionNum == XCPT_ACCESS_VIOLATION
        && ( p1->ExceptionInfo[ 0 ] & XCPT_WRITE_ACCESS ))
    {
        /* Exception address */
        void *pStart = ( void * )p1->ExceptionInfo[ 1 ];

        if( reallocDestMem( pStart, ( char * )pStart + 1, true ))
            return XCPT_CONTINUE_EXECUTION; /* Continue to execution */
    }

    /* Not processed or other signal exception occurred */

    /* Continue to search other exception handler */
    return XCPT_CONTINUE_SEARCH;
}

/** __init_app() of kLIBC. */
static void ( *klibc___init_app )( void ) = NULL;

/** _beginthread() of kLIBC. */
static int ( *klibc__beginthread )
    ( void ( * )( void * ), void *, unsigned, void * )= NULL;

/**
 * Load functions of kLIBC
 */
__attribute__(( unused )) /* Make compiler happy */
static void loadKLibcFuncs( void )
{
    char szFailed[ CCHMAXPATH ];
    HMODULE hmod;

    if( DosLoadModule( szFailed, sizeof( szFailed ), "libc066", &hmod ))
        DosExit( EXIT_PROCESS, 255 );

    if( DosQueryProcAddr( hmod, 0, "___init_app", ( PFN * )&klibc___init_app ))
        DosExit( EXIT_PROCESS, 255 );

    if( DosQueryProcAddr( hmod, 0, "__beginthread",
                          ( PFN * )&klibc__beginthread ))
        DosExit( EXIT_PROCESS, 255 );
}

/**
 * structure for a stack frame for main() of kLIBC
 *
 * @remark Copied from lib/sys/__init.c of kLIBC.
 */
typedef struct _STACKFRAME
{
    /** Argument count. */
    int                           argc;
    /** Pointer to argument vector. */
    char **                       argv;
    /** Pointer to environmet vector. */
    char **                       envp;
    /** Exception handler registration record (*_sys_xreg). */
    EXCEPTIONREGISTRATIONRECORD   ExcpRegRec;
    /** Argument vector. */
    char *                        apszArg[1];
    /** somewhere after this comes the string. */
} STACKFRAME, *PSTACKFRAME;

__attribute__(( unused ))   /* Make compiler happy */
static long L_ret = 0;      /**< Return address of the caller. */

/**
 * Register an exception handler for SIGSEGV and generate a new stack frame
 * for main()
 *
 * @param[in] Stack frame generated by kLIBC for main().
 * @see lib/sys/__init.c and lib/sys/386/appinit.s of kLIBC.
 */
__attribute__(( unused )) /* Make compiler happy */
static void preMain( const PSTACKFRAME psf )
{
    EXCEPTIONREGISTRATIONRECORD xcptRec = { 0, };

    /* Register an exception handler for SIGSEGV. */
    xcptRec.ExceptionHandler = ( ERR )sigsegv;
    DosSetExceptionHandler( &xcptRec );

    /* Generate a new stack frame for main() and return to the caller. */
    __asm__ __volatile__(
        "pushl  %2              # pass envp                 \n\t"
        "pushl  %1              # pass argv                 \n\t"
        "pushl  %0              # pass argc                 \n\t"
        "# Now esp points to a new stack frame for main()   \n\t"
        "jmp    *_L_ret         # return to the caller      \n\t"
        : /* No outputs */
        : "g"( psf->argc ), "g"( psf->argv ), "g"( psf->envp )
    );
}

/**
 * Replacement of __init_app() of kLIBC
 *
 * @see lib/sys/386/appinit.s of kLIBC.
 */
void __init_app( void )
{
    __asm__ __volatile__(
        "pushl  4(%ebp)             # push a return address         \n\t"
        "popl   _L_ret              # store it                      \n\t"
        "call   _loadKLibcFuncs     # load kLIBC functions          \n\t"
        "call   *_klibc___init_app  # call __init_app() of kLIBC    \n\t"
        "# Now esp points to a stack frame for main()               \n\t"
        "pushl  %esp                # pass struct stackframe *      \n\t"
        "call   _preMain            # call preMain()                \n\t"
        "# Never should get here                                    \n\t"
        "hlt                        # SIGSEGV                       \n\t"
    );
}

/**
 * structure for thread arguments
 */
typedef struct _THREADARG
{
    void ( *pfnStart )( void *arg );    /**< Start function. */
    void *pArgList;                     /**< Arguments list. */
} THREADARG, *PTHREADARG;

/**
 * Thread entry
 */
void threadEntry( void *arg )
{
    PTHREADARG pta = ( PTHREADARG )arg;

    EXCEPTIONREGISTRATIONRECORD regRec = { 0 };

    /* Register an exception handler for SIGSEGV */
    regRec.ExceptionHandler = ( ERR )sigsegv;
    DosSetExceptionHandler( &regRec );

    /* Call passed start function with passed arguments */
    pta->pfnStart( pta->pArgList );

    /* Deregister an exception handler */
    DosUnsetExceptionHandler( &regRec );

    /* Free passed thread arguments */
    free( pta );
}

/**
 * Replacement of _beginthread() of kLIBC
 */
int _beginthread( void ( *start )( void *arg ), void *stack,
                  unsigned stack_size, void *arg_list )
{
    PTHREADARG pta;

    /* Allocate memory for thread arguments */
    pta = malloc( sizeof( *pta ));
    if( !pta )
        return -1;

    /* Pass start function and arguments list via thread arguments */
    pta->pfnStart = start;
    pta->pArgList = arg_list;

    /* Call _beginthread() of kLIBC */
    return ( *klibc__beginthread )( threadEntry, stack, stack_size, pta );
}

/**
 * Perform copy-on-write
 *
 * @param[in] p  Pointer to the source memory for copy-on-write.
 * @param[in] cb Bytes to copy-on-write.
 * @return Pointer to the destination memory for copy-on-write.
 * @remark When source memory is freed, destination memories are not
 *         reallocated.
 */
void *copyOnWrite( const void *p, int cb )
{
    ULONG cbSrc = cb;
    ULONG flSrc;
    PMEMORYLIST pmlNew;
    PMEMORYLIST pmlNewSrc;
    void *pAlias;

    /* Page aligned ? */
    if(( intptr_t )p % getpagesize())
        return NULL;

    if( DosQueryMem( &p, &cbSrc, &flSrc ))
        return NULL;

    /* Prohibit WRITE access of source memory */
    flSrc &= fPERM;
    if( DosSetMem( p, cb, flSrc & ~PAG_WRITE ))
        return NULL;

    if( DosAliasMem( p, cb, &pAlias, 0 ))
        return NULL;

    /* Prohibit WRITE access */
    if( DosSetMem( pAlias, cb, fPERM & ~PAG_WRITE )
        || !( pmlNew = malloc( sizeof( *pmlNew )))
        || !( pmlNewSrc = malloc( sizeof( *pmlNewSrc ))))
    {
        free( pmlNewSrc );
        free( pmlNew );
        DosFreeMem( pAlias );

        return NULL;
    }

    /* Add a memory list */
    pmlNew->pDest = pAlias;
    pmlNew->fl = 0;
    pmlNew->cb = cb;
    pmlNew->pSrc = p;
    pmlNew->pmlNext = pmlStart;

    pmlStart = pmlNew;

    /* Add a memory list for source memory */
    pmlNewSrc->pDest = ( void * )p;
    pmlNewSrc->fl = flSrc;
    pmlNewSrc->cb = cb;
    pmlNewSrc->pSrc = NULL;
    pmlNewSrc->pmlNext = pmlStart;

    pmlStart = pmlNewSrc;

    return pAlias;
}
