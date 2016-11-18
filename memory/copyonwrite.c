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
static PMEMORYLIST m_pmlStart = NULL;

/** DosFreeMem() of DOSCALLS */
static APIRET APIENTRY ( *m_os2_DosFreeMem )( PVOID ) = NULL;

/** DosSetMem() of DOSCALLS */
static APIRET APIENTRY ( *m_os2_DosSetMem )( CPVOID, ULONG, ULONG ) = NULL;

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
    PMEMORYLIST *pmlNext = &m_pmlStart;

    /* Find releated memory list and perform copy */
    for( pml = m_pmlStart; pml; pml = *pmlNext )
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
                if( m_os2_DosFreeMem( pml->pDest ))
                    break;

                if( DosAllocMemEx( &pml->pDest, pml->cb,
                                   fPERM | PAG_COMMIT | OBJ_LOCATION ))
                    break;

                memcpy( pml->pDest, pml->pSrc, pml->cb );
            }
            else
            {
                /* No, source memory */

                /* Just restore permission flag if having WRITE access */
                if( !( pml->fl & PAG_WRITE )
                    || m_os2_DosSetMem( pml->pDest, pml->cb, pml->fl ))
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
static void ( *m_klibc___init_app )( void ) = NULL;

/** _beginthread() of kLIBC. */
static int ( *m_klibc__beginthread )
    ( void ( * )( void * ), void *, unsigned, void * ) = NULL;

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

    if( DosQueryProcAddr( hmod, 0, "___init_app",
                        ( PFN * )&m_klibc___init_app ))
        DosExit( EXIT_PROCESS, 255 );

    if( DosQueryProcAddr( hmod, 0, "__beginthread",
                          ( PFN * )&m_klibc__beginthread ))
        DosExit( EXIT_PROCESS, 255 );
}

/**
 * Load functions of OS/2
 */
__attribute__(( unused )) /* Make compiler happy */
static void loadOS2Funcs( void )
{
    char szFailed[ CCHMAXPATH ];
    HMODULE hmod;

    if( DosLoadModule( szFailed, sizeof( szFailed ), "doscalls", &hmod ))
        DosExit( EXIT_PROCESS, 255 );

    if( DosQueryProcAddr( hmod, 304, NULL, ( PFN * )&m_os2_DosFreeMem ))
        DosExit( EXIT_PROCESS, 255 );

    if( DosQueryProcAddr( hmod, 305, NULL, ( PFN * )&m_os2_DosSetMem ))
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
static long m_L_ret = 0;    /**< Return address of the caller. */

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
        "jmp    *_m_L_ret       # return to the caller      \n\t"
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
        "popl   _m_L_ret            # store it                      \n\t"
        "call   _loadKLibcFuncs     # load kLIBC functions          \n\t"
        "call   _loadOS2Funcs       # load OS/2 functions           \n\t"
        "call   *_m_klibc___init_app# call __init_app() of kLIBC    \n\t"
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
    return ( *m_klibc__beginthread )( threadEntry, stack, stack_size, pta );
}

/**
 * Calculate size of given memory object
 *
 * @param[in] p Base address of a memory object.
 * @return Size of given memory object on success, otherwise -1.
 * @remark Returned size is not match exactly with actual size of the given
 *         memory object. Because a reserved memory, that is,
 *         without PAG_COMMIT nor PAG_FREE, is included as well as
 *         PAG_DECOMMIT.
 */
static int calcMemObjSize( const void *p )
{
    int size;

    int pagesize = getpagesize();

    ULONG cb = pagesize;
    ULONG fl;

    if( DosQueryMem( p, &cb, &fl ) || !( fl & PAG_BASE ))
        return -1;

    for( size = cb; ; size += cb )
    {
        cb = pagesize;

        if( DosQueryMem(( char * )p + size, &cb, &fl ))
            break;

        if( fl & ( PAG_BASE | PAG_FREE ))
            break;
    }

    return size;
}

/**
 * Replacement of DosFreeMem() of DOSCALLS
 *
 * @remark If given @a pb is the first page( PAG_BASE), reallocate affected
 *         memories unconditionally regardless of success of DosFreeMem().
 */
APIRET APIENTRY DosFreeMem( PVOID pb )
{
    int size = calcMemObjSize( pb );

    if( size != -1 )
        reallocDestMem( pb, ( char * )pb + size, true );

    return m_os2_DosFreeMem( pb );
}

/**
 * Replacement of DosSetMem() of DOSCALLS
 *
 * @remark If PAG_DECOMMIT is given, reallocate affected memories
 *         unconditionally regardless of success of DosSetMem( PAG_DECOMMIT ).
 */
APIRET APIENTRY DosSetMem( CPVOID pb, ULONG cb, ULONG fl )
{
    if( fl & PAG_DECOMMIT )
    {
        int pagesize_1 = getpagesize() - 1;

        /* Align with pagesize */
        void *pStart = ( void * )(( intptr_t )pb & ~pagesize_1 );
        void *pEnd = ( char * )pStart + (( cb + pagesize_1 ) & ~pagesize_1 );

        reallocDestMem( pStart, pEnd, true );
    }

    return m_os2_DosSetMem( pb, cb, fl );
}

/**
 * Perform copy-on-write
 *
 * @param[in] p  Pointer to the source memory for copy-on-write.
 * @param[in] cb Bytes to copy-on-write.
 * @return Pointer to the destination memory for copy-on-write.
 * @bug If permission of memory is changed, the change is not applied.
 *      For example,
 *        1. allocate a memory with PAG_WRITE
 *        2. perform copyOnWrite() on it
 *        3. remove PAG_WRITE of it with DosSetMem()
 *        4. try to write to it
 *        5. this should cause SIGSEGV.
 *        6. however, its PAG_WRITE is restored in sigsegv()
 *        7. as a result, SIGSEGV does not occur
 *        8. this is not expected behavior
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
    if( m_os2_DosSetMem( p, cb, flSrc & ~PAG_WRITE ))
        return NULL;

    if( DosAliasMem( p, cb, &pAlias, 0 ))
        return NULL;

    /* Prohibit WRITE access */
    if( m_os2_DosSetMem( pAlias, cb, fPERM & ~PAG_WRITE )
        || !( pmlNew = malloc( sizeof( *pmlNew )))
        || !( pmlNewSrc = malloc( sizeof( *pmlNewSrc ))))
    {
        free( pmlNewSrc );
        free( pmlNew );
        m_os2_DosFreeMem( pAlias );

        return NULL;
    }

    /* Add a memory list */
    pmlNew->pDest = pAlias;
    pmlNew->fl = 0;
    pmlNew->cb = cb;
    pmlNew->pSrc = p;
    pmlNew->pmlNext = m_pmlStart;

    m_pmlStart = pmlNew;

    /* Add a memory list for source memory */
    pmlNewSrc->pDest = ( void * )p;
    pmlNewSrc->fl = flSrc;
    pmlNewSrc->cb = cb;
    pmlNewSrc->pSrc = NULL;
    pmlNewSrc->pmlNext = m_pmlStart;

    m_pmlStart = pmlNewSrc;

    return pAlias;
}
