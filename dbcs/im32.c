/*
 * OS/2 32 bit Input Method dynamic loader
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOSMODULEMGR
#include <os2.h>
#include <os2im.h>
#include <os2ime.h>

#include "im32.h"

typedef APIRET ( APIENTRY *PFN_IMASSOCIATEINSTANCE )
                    ( HWND hwnd, HIMI himi, PHIMI phimiPrev );

typedef APIRET ( APIENTRY *PFN_IMCREATEINSTANCE )( PHIMI phimi );

typedef APIRET ( APIENTRY *PFN_IMDESTROYINSTANCE )( HIMI himi );

typedef APIRET ( APIENTRY *PFN_IMGETINSTANCE )( HWND hwnd, PHIMI phimi );

typedef APIRET ( APIENTRY *PFN_IMQUERYDEFAULTIMWINDOW )
                    ( HWND hwnd, PHWND phwnd );

typedef APIRET ( APIENTRY *PFN_IMRELEASEINSTANCE )( HWND hwnd, HIMI himi );

typedef APIRET ( APIENTRY *PFN_IMENUMREGISTERWORD )
                    ( HIMI himi, REGISTERWORDENUMPROC pfnEnumProc,
                      PSZ pReading, ULONG ulType, PSZ pRegister, PVOID pData );

typedef APIRET ( APIENTRY *PFN_IMQUERYREGISTERWORDTYPE )
                    ( HIMI himi, PULONG pulCount, PWORDTYPE pWordType );

typedef APIRET ( APIENTRY *PFN_IMREGISTERWORD )
                    ( HIMI himi, PSZ pszReading, ULONG ulType,
                      PSZ pszRegister );

typedef APIRET ( APIENTRY *PFN_IMDEREGISTERWORD )
                    ( HIMI himi, PSZ pszReading, ULONG ulType,
                      PSZ pszDeregister );

typedef APIRET ( APIENTRY *PFN_IMSHOWIMEDLG )
                    ( HIMI himi, ULONG ulDlgType,
                      PREGISTERWORDHEADER pRegWord );

typedef APIRET ( APIENTRY *PFN_IMESCAPE )
                    ( HIMI himi, ULONG ulEscaoe, PVOID pData );

typedef APIRET ( APIENTRY *PFN_IMQUERYCANDIDATELIST )
                    ( HIMI himi, ULONG ulIndex,
                      PCANDIDATELISTHEADER pCandidateList, PULONG pulBuffer );

typedef APIRET ( APIENTRY *PFN_IMQUERYCANDIDATEWINDOWPOS )
                    ( HIMI himi, PCANDIDATEPOS pCandidatePos );

typedef APIRET ( APIENTRY *PFN_IMSETCANDIDATEWINDOWPOS )
                    ( HIMI himi, PCANDIDATEPOS pCandidatePos );

typedef APIRET ( APIENTRY *PFN_IMQUERYCONVERSIONFONT )
                    ( HIMI himi, PFATTRS pFontAttrs );

typedef APIRET ( APIENTRY *PFN_IMSETCONVERSIONFONT )
                    ( HIMI himi, PFATTRS pFontAttrs );

typedef APIRET ( APIENTRY *PFN_IMQUERYCONVERSIONFONTSIZE )
                    ( HIMI himi, PSIZEF psizfxBox );

typedef APIRET ( APIENTRY *PFN_IMSETCONVERSOINFONTSIZE )
                    ( HIMI himi, PSIZEF psizfxBox );

typedef APIRET ( APIENTRY *PFN_IMGETCONVERSIONSTRING )
                    ( HIMI himi, ULONG ulIndex, PVOID pBuf, PULONG pulBufLen );

typedef APIRET ( APIENTRY *PFN_IMSETCONVERSIONSTRING )
                    ( HIMI himi, ULONG ulIndex, PVOID pConv, ULONG ulConvLen,
                      PVOID pReading, ULONG ulReadingLen );

typedef APIRET ( APIENTRY *PFN_IMQUERYCONVERSIONANGLE )
                    ( HIMI himi, PGRADIENTL pgradlAngle );

typedef APIRET ( APIENTRY *PFN_IMSETCONVERSIONANGLE )
                    ( HIMI himi, PGRADIENTL pgradlAngle );

typedef APIRET ( APIENTRY *PFN_IMQUERYCONVERSIONWINDOWPOS )
                    ( HIMI himi, PCONVERSIONPOS pConvPos );

typedef APIRET ( APIENTRY *PFN_IMSETCONVERSIONWINDOWPOS )
                    ( HIMI himi, PCONVERSIONPOS pConvPos );

typedef APIRET ( APIENTRY *PFN_IMGETRESULTSTRING )
                    ( HIMI himi, ULONG ulIndex, PVOID pBuf, PULONG pulBufLen );

typedef APIRET ( APIENTRY *PFN_IMQUERYINFOMSG )
                    ( HIMI himi, ULONG ulIndex, PVOID pBuf, PULONG pulBufLen );

typedef APIRET ( APIENTRY *PFN_IMQUERYSTATUSWINDOWPOS )
                    ( HIMI himi, PPOINTL pptPos, PSIZEL pszlSize );

typedef APIRET ( APIENTRY *PFN_IMSETSTATUSWINDOWPOS )
                    ( HIMI himi, PPOINTL pptPos );

typedef APIRET ( APIENTRY *PFN_IMSHOWSTATUSWINDOW )( HIMI himi, ULONG fShow );

typedef APIRET ( APIENTRY *PFN_IMGETSTATUSSTRING )
                    ( HIMI himi, ULONG ulIndex, PVOID pBuf, PULONG pulBufLen );

typedef APIRET ( APIENTRY *PFN_IMCONVERTSTRING )
                    ( HIMI himi, PSZ pSrc, PCANDIDATELISTHEADER pDst,
                      PULONG pulBufLen, ULONG ulFlag );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMMODE )
                    ( HIMI himi, PULONG pulInputMode,
                      PULONG pulConversionMode );

typedef APIRET ( APIENTRY *PFN_IMSETIMMODE )
                    ( HIMI himi, ULONG ulInputMode, ULONG ulConversionMode );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMEINFO )
                    ( ULONG ImeId, PIMEINFOHEADER pImeInfoHeader,
                      PULONG pulBufLen );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMEPROPERTY )
                    ( HIMI himi, ULONG ulIndex, PULONG pulProp );

typedef APIRET ( APIENTRY *PFN_IMREQUESTIME )
                    ( HIMI himi, ULONG ulAction, ULONG ulIndex,
                      ULONG ulValue );

typedef APIRET ( APIENTRY *PFN_IMISIMEMESSAGE )
                    ( HWND hwndIm, ULONG msg, MPARAM mp1, MPARAM mp2,
                      PBOOL pfResult );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMELIST )
                    ( ULONG ulCodepage, PULONG aImeId, PULONG pulCount );

typedef APIRET ( APIENTRY *PFN_IMQUERYCURRENTIME )( HIMI himi, PULONG pImeId );

typedef APIRET ( APIENTRY *PFN_IMSETCURRENTIME )
                    ( HIMI himi, ULONG ulAction, ULONG ImeId );

typedef APIRET ( APIENTRY *PFN_IMQUERYDEFAULTIME )
                    ( ULONG ulCodepage, PULONG pImeId );

typedef APIRET ( APIENTRY *PFN_IMSETDEFAULTIME )
                    ( ULONG ulCodepage, ULONG ImeId );


typedef APIRET ( APIENTRY *PFN_IMSETIMELISTORDER )
                    ( ULONG ulCodepage, ULONG ulAction, ULONG ImeIdTarget,
                      ULONG ImeIdIndex );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMEID )( PSZ pszIMEName, PULONG pImeId );

typedef APIRET ( APIENTRY *PFN_IMQUERYMSGQUEUEPROPERTY )
                    ( HMQ hmq, PULONG pulFlag );

typedef APIRET ( APIENTRY *PFN_IMSETMSGQUEUEPROPERTY )
                    ( HMQ hmq, ULONG ulFlag );

/* Input Method Interfaces for IME */

typedef APIRET ( APIENTRY *PFN_IMREQUESTEVENT)
                    ( HIMI hImi, ULONG msg, ULONG mp1, ULONG mp2 );

typedef APIRET ( APIENTRY *PFN_IMREQUESTIMINSTANCE)
                    ( HIMI hImi, PIMINSTANCE * ppIMInstance );

typedef APIRET ( APIENTRY *PFN_IMRELEASEIMINSTANCE)( HIMI hImi );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMINSTANCEREQCOUNT)
                    ( HIMI hImi, PULONG pulCount );

typedef APIRET ( APIENTRY *PFN_IMCREATEIMIPART)
                    ( ULONG ulPartSize, PHIMIP phimiPart );

typedef APIRET ( APIENTRY *PFN_IMDESTROYIMIPART)( HIMIP himiPart );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMIPARTSIZE)
                    ( HIMIP himiPart, PULONG pulPartSize );

typedef APIRET ( APIENTRY *PFN_IMREQUESTIMIPART)
                    ( HIMIP himiPart, PVOID * ppPartData );

typedef APIRET ( APIENTRY *PFN_IMRELEASEIMIPART)( HIMIP himiPart );

typedef APIRET ( APIENTRY *PFN_IMRESIZEIMIPART)
                    ( HIMIP himiPart, ULONG ulNewSize );

typedef APIRET ( APIENTRY *PFN_IMQUERYIMIPARTREQCOUNT)
                    ( HIMIP himiPart, PULONG pulCount );


typedef APIRET ( APIENTRY *PFN_IMBROADCASTDATA)
                    ( PSZ pszIMEName, ULONG ulIndex,
                      PBROADCASTDATAHEADER pData );

static BOOL fIM32Inited = FALSE;

static HMODULE hIM32Mod = NULLHANDLE;

static PFN_IMASSOCIATEINSTANCE pfnImAssociateInstance = NULL;

static PFN_IMCREATEINSTANCE pfnImCreateInstance = NULL;

static PFN_IMDESTROYINSTANCE pfnImDestroyInstance = NULL;

static PFN_IMGETINSTANCE pfnImGetInstance = NULL;

static PFN_IMQUERYDEFAULTIMWINDOW pfnImQueryDefaultIMWindow = NULL;

static PFN_IMRELEASEINSTANCE pfnImReleaseInstance = NULL;

static PFN_IMENUMREGISTERWORD pfnImEnumRegisterWord = NULL;

static PFN_IMQUERYREGISTERWORDTYPE pfnImQueryRegisterWordType = NULL;

static PFN_IMREGISTERWORD pfnImRegisterWord = NULL;

static PFN_IMDEREGISTERWORD pfnImDeregisterWord = NULL;

static PFN_IMSHOWIMEDLG pfnImShowIMEDlg = NULL;

static PFN_IMESCAPE pfnImEscape = NULL;

static PFN_IMQUERYCANDIDATELIST pfnImQueryCandidateList = NULL;

static PFN_IMQUERYCANDIDATEWINDOWPOS pfnImQueryCandidateWindowPos = NULL;

static PFN_IMSETCANDIDATEWINDOWPOS pfnImSetCandidateWindowPos = NULL;

static PFN_IMQUERYCONVERSIONFONT pfnImQueryConversionFont = NULL;

static PFN_IMSETCONVERSIONFONT pfnImSetConversionFont = NULL;

static PFN_IMQUERYCONVERSIONFONTSIZE pfnImQueryConversionFontSize = NULL;

static PFN_IMSETCONVERSOINFONTSIZE pfnImSetConversionFontSize = NULL;

static PFN_IMGETCONVERSIONSTRING pfnImGetConversionString = NULL;

static PFN_IMSETCONVERSIONSTRING pfnImSetConversionString = NULL;

static PFN_IMQUERYCONVERSIONANGLE pfnImQueryConversionAngle = NULL;

static PFN_IMSETCONVERSIONANGLE pfnImSetConversionAngle = NULL;

static PFN_IMQUERYCONVERSIONWINDOWPOS pfnImQueryConversionWindowPos = NULL;

static PFN_IMSETCONVERSIONWINDOWPOS pfnImSetConversionWindowPos = NULL;

static PFN_IMGETRESULTSTRING pfnImGetResultString = NULL;

static PFN_IMQUERYINFOMSG pfnImQueryInfoMsg = NULL;

static PFN_IMQUERYSTATUSWINDOWPOS pfnImQueryStatusWindowPos = NULL;

static PFN_IMSETSTATUSWINDOWPOS pfnImSetStatusWindowPos = NULL;

static PFN_IMSHOWSTATUSWINDOW pfnImShowStatusWindow = NULL;

static PFN_IMGETSTATUSSTRING pfnImGetStatusString = NULL;

static PFN_IMCONVERTSTRING pfnImConvertString = NULL;

static PFN_IMQUERYIMMODE pfnImQueryIMMode = NULL;

static PFN_IMSETIMMODE pfnImSetIMMode = NULL;

static PFN_IMQUERYIMEINFO pfnImQueryIMEInfo = NULL;

static PFN_IMQUERYIMEPROPERTY pfnImQueryIMEProperty = NULL;

static PFN_IMREQUESTIME pfnImRequestIME = NULL;

static PFN_IMISIMEMESSAGE pfnImIsIMEMessage = NULL;

static PFN_IMQUERYIMELIST pfnImQueryIMEList = NULL;

static PFN_IMQUERYCURRENTIME pfnImQueryCurrentIME = NULL;

static PFN_IMSETCURRENTIME pfnImSetCurrentIME = NULL;

static PFN_IMQUERYDEFAULTIME pfnImQueryDefaultIME = NULL;

static PFN_IMSETDEFAULTIME pfnImSetDefaultIME = NULL;

static PFN_IMSETIMELISTORDER pfnImSetIMEListOrder = NULL;

static PFN_IMQUERYIMEID pfnImQueryIMEID = NULL;

static PFN_IMQUERYMSGQUEUEPROPERTY pfnImQueryMsgQueueProperty = NULL;

static PFN_IMSETMSGQUEUEPROPERTY pfnImSetMsgQueueProperty = NULL;

/* Input Method Interfaces for IME */

static PFN_IMREQUESTEVENT pfnImRequestEvent = NULL;

static PFN_IMREQUESTIMINSTANCE pfnImRequestIMInstance = NULL;

static PFN_IMRELEASEIMINSTANCE pfnImReleaseIMInstance = NULL;

static PFN_IMQUERYIMINSTANCEREQCOUNT pfnImQueryIMInstanceReqCount = NULL;

static PFN_IMCREATEIMIPART pfnImCreateIMIPart = NULL;

static PFN_IMDESTROYIMIPART pfnImDestroyIMIPart = NULL;

static PFN_IMQUERYIMIPARTSIZE pfnImQueryIMIPartSize = NULL;

static PFN_IMREQUESTIMIPART pfnImRequestIMIPart = NULL;

static PFN_IMRELEASEIMIPART pfnImReleaseIMIPart = NULL;

static PFN_IMRESIZEIMIPART pfnImResizeIMIPart = NULL;

static PFN_IMQUERYIMIPARTREQCOUNT pfnImQueryIMIPartReqCount = NULL;

static PFN_IMBROADCASTDATA pfnImBroadcastData = NULL;

#define DOSQUERYPROCADDR( ordinal, ppfn ) \
    DosQueryProcAddr( hIM32Mod, ordinal, NULL, ( PFN * )( ppfn ))

#ifdef __GNUC__
#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR  __attribute__((destructor))
#else
#define CONSTRUCTOR
#define DESTRUCTOR
#endif

CONSTRUCTOR int im32Init( VOID )   /* Called at startup if supported */
{
    UCHAR szErrorName[ 256 ];

    if( fIM32Inited )
        return TRUE;

    hIM32Mod = NULLHANDLE;

    if( DosLoadModule( szErrorName, sizeof( szErrorName ), "os2im",
                       &hIM32Mod ))
        return fIM32Inited;

    if( DOSQUERYPROCADDR( 101, &pfnImAssociateInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 102, &pfnImCreateInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 103, &pfnImDestroyInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 104, &pfnImGetInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 105, &pfnImQueryDefaultIMWindow ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 106, &pfnImReleaseInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 107, &pfnImEnumRegisterWord ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 108, &pfnImQueryRegisterWordType ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 109, &pfnImRegisterWord ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 110, &pfnImDeregisterWord ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 111, &pfnImShowIMEDlg ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 112, &pfnImEscape ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 113, &pfnImQueryCandidateList ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 114, &pfnImQueryCandidateWindowPos ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 115, &pfnImSetCandidateWindowPos ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 116, &pfnImQueryConversionFont ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 117, &pfnImSetConversionFont ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 118, &pfnImGetConversionString ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 119, &pfnImSetConversionString ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 120, &pfnImQueryConversionWindowPos ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 121, &pfnImSetConversionWindowPos ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 122, &pfnImGetResultString ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 142, &pfnImQueryConversionFontSize ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 144, &pfnImSetConversionFontSize ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 141, &pfnImQueryConversionAngle ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 143, &pfnImSetConversionAngle ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 123, &pfnImQueryInfoMsg ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 124, &pfnImQueryStatusWindowPos ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 125, &pfnImSetStatusWindowPos ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 145, &pfnImShowStatusWindow ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 149, &pfnImGetStatusString ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 126, &pfnImConvertString ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 127, &pfnImQueryIMMode ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 128, &pfnImSetIMMode ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 129, &pfnImQueryIMEInfo ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 130, &pfnImQueryIMEProperty ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 131, &pfnImRequestIME ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 132, &pfnImIsIMEMessage ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 135, &pfnImQueryIMEList ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 136, &pfnImQueryCurrentIME ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 137, &pfnImSetCurrentIME ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 147, &pfnImQueryDefaultIME ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 148, &pfnImSetDefaultIME ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 138, &pfnImSetIMEListOrder ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 146, &pfnImQueryIMEID ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 139, &pfnImQueryMsgQueueProperty ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 140, &pfnImSetMsgQueueProperty ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 511, &pfnImRequestEvent ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 501, &pfnImRequestIMInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 502, &pfnImReleaseIMInstance ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 503, &pfnImQueryIMInstanceReqCount ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 504, &pfnImCreateIMIPart ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 505, &pfnImDestroyIMIPart ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 506, &pfnImQueryIMIPartSize ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 507, &pfnImRequestIMIPart ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 508, &pfnImReleaseIMIPart ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 509, &pfnImResizeIMIPart ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 510, &pfnImQueryIMIPartReqCount ))
        goto error_exit;

    if( DOSQUERYPROCADDR( 514, &pfnImBroadcastData ))
        goto error_exit;

    fIM32Inited = TRUE;

error_exit:

    if( hIM32Mod && !fIM32Inited )
        DosFreeModule( hIM32Mod );

    return fIM32Inited;
}

DESTRUCTOR void im32Term( void )    /* Called at exit if supported */
{
    if( !fIM32Inited )
        return;

    DosFreeModule( hIM32Mod );

    fIM32Inited = FALSE;
}

int im32Inited( void )
{
    return fIM32Inited;
}

APIRET APIENTRY ImAssociateInstance( HWND hwnd, HIMI himi, PHIMI phimiPrev )
{
    if( !fIM32Inited )
        return 1;

    return pfnImAssociateInstance( hwnd, himi, phimiPrev );
}

APIRET APIENTRY ImCreateInstance( PHIMI phimi )
{
    if( !fIM32Inited )
        return 1;

    return pfnImCreateInstance( phimi );
}

APIRET APIENTRY ImDestroyInstance( HIMI himi )
{
    if( !fIM32Inited )
        return 1;

    return pfnImDestroyInstance( himi );
}


APIRET APIENTRY ImGetInstance( HWND hwnd, PHIMI phimi )
{
    if( !fIM32Inited )
        return 1;

    return pfnImGetInstance( hwnd, phimi );
}

APIRET APIENTRY ImQueryDefaultIMWindow( HWND hwnd, PHWND phwnd )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryDefaultIMWindow( hwnd, phwnd );
}

APIRET APIENTRY ImReleaseInstance( HWND hwnd, HIMI himi )
{
    if( !fIM32Inited )
        return 1;

    return pfnImReleaseInstance( hwnd, himi );
}

APIRET APIENTRY ImEnumRegisterWord( HIMI himi,
                                    REGISTERWORDENUMPROC pfnEnumProc,
                                    PSZ pReading,
                                    ULONG ulType,
                                    PSZ pRegister,
                                    PVOID pData )
{
    if( !fIM32Inited )
        return 1;

    return pfnImEnumRegisterWord( himi, pfnEnumProc, pReading, ulType, pRegister, pData );
}

APIRET APIENTRY ImQueryRegisterWordType( HIMI himi, PULONG pulCount,
                                         PWORDTYPE pWordType )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryRegisterWordType( himi, pulCount, pWordType );
}


APIRET APIENTRY ImRegisterWord( HIMI himi,
                                PSZ pszReading,
                                ULONG ulType,
                                PSZ pszRegister )
{
    if( !fIM32Inited )
        return 1;

    return pfnImRegisterWord( himi, pszReading, ulType, pszRegister );
}

APIRET APIENTRY ImDeregisterWord( HIMI himi,
                                  PSZ pszReading,
                                  ULONG ulType,
                                  PSZ pszDeregister )
{
    if( !fIM32Inited )
        return 1;

    return pfnImDeregisterWord( himi, pszReading, ulType, pszDeregister );
}

APIRET APIENTRY ImShowIMEDlg( HIMI himi,
                             ULONG ulDlgType,
                             PREGISTERWORDHEADER pRegWord )
{
    if( !fIM32Inited )
        return 1;

    return pfnImShowIMEDlg( himi, ulDlgType, pRegWord );
}

APIRET APIENTRY ImEscape( HIMI himi, ULONG ulEscape, PVOID pData )
{
    if( !fIM32Inited )
        return 1;

    return pfnImEscape( himi, ulEscape, pData );
}

APIRET APIENTRY ImQueryCandidateList( HIMI himi,
                                      ULONG ulIndex,
                                      PCANDIDATELISTHEADER pCandidateList,
                                      PULONG pulBuffer )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryCandidateList( himi, ulIndex, pCandidateList, pulBuffer );
}

APIRET APIENTRY ImQueryCandidateWindowPos( HIMI himi,
                                           PCANDIDATEPOS pCandidatePos )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryCandidateWindowPos( himi, pCandidatePos );
}

APIRET APIENTRY ImSetCandidateWindowPos( HIMI himi,
                                         PCANDIDATEPOS pCandidatePos )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetCandidateWindowPos( himi, pCandidatePos );
}

APIRET APIENTRY ImQueryConversionFont( HIMI himi,
                                       PFATTRS pFontAttrs )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryConversionFont( himi, pFontAttrs );
}

APIRET APIENTRY ImSetConversionFont( HIMI himi,
                                     PFATTRS pFontAttrs )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetConversionFont( himi, pFontAttrs );
}

APIRET APIENTRY ImQueryConversionFontSize( HIMI himi,
                                           PSIZEF psizfxBox )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryConversionFontSize( himi, psizfxBox );
}

APIRET APIENTRY ImSetConversionFontSize( HIMI himi,
                                         PSIZEF psizfxBox )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetConversionFontSize( himi, psizfxBox );
}

APIRET APIENTRY ImGetConversionString( HIMI himi, ULONG ulIndex,
                                       PVOID pBuf, PULONG pulBufLen )
{
    if( !fIM32Inited )
        return 1;

    return pfnImGetConversionString( himi, ulIndex, pBuf, pulBufLen );
}

APIRET APIENTRY ImSetConversionString( HIMI himi, ULONG ulIndex,
                                         PVOID pConv, ULONG ulConvLen,
                                         PVOID pReading, ULONG ulReadingLen )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetConversionString( himi, ulIndex, pConv, ulConvLen, pReading, ulReadingLen );
}

APIRET APIENTRY ImQueryConversionAngle( HIMI himi, PGRADIENTL pgradlAngle )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryConversionAngle( himi, pgradlAngle );
}

APIRET APIENTRY ImSetConversionAngle( HIMI himi, PGRADIENTL pgradlAngle )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetConversionAngle( himi, pgradlAngle );
}

APIRET APIENTRY ImQueryConversionWindowPos( HIMI himi,
                                            PCONVERSIONPOS pConvPos )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryConversionWindowPos( himi, pConvPos );
}


APIRET APIENTRY ImSetConversionWindowPos( HIMI himi,
                                          PCONVERSIONPOS pConvPos )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetConversionWindowPos( himi, pConvPos );
}

APIRET APIENTRY ImGetResultString( HIMI himi, ULONG ulIndex,
                                   PVOID pBuf, PULONG pulBufLen )
{
    if( !fIM32Inited )
        return 1;

    return pfnImGetResultString( himi, ulIndex, pBuf, pulBufLen );
}

APIRET APIENTRY ImQueryInfoMsg( HIMI himi, ULONG ulIndex,
                                     PVOID pBuf, PULONG pulBufLen )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryInfoMsg( himi, ulIndex, pBuf, pulBufLen );
}

APIRET APIENTRY ImQueryStatusWindowPos( HIMI himi, PPOINTL pptPos, PSIZEL pszlSize )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryStatusWindowPos( himi, pptPos, pszlSize );
}

APIRET APIENTRY ImSetStatusWindowPos( HIMI himi, PPOINTL pptPos )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetStatusWindowPos( himi, pptPos );
}

APIRET APIENTRY ImShowStatusWindow( HIMI himi, ULONG fShow )
{
    if( !fIM32Inited )
        return 1;

    return pfnImShowStatusWindow( himi, fShow );
}

APIRET APIENTRY ImGetStatusString( HIMI himi, ULONG ulIndex,
                                   PVOID pBuf, PULONG pulBufLen )
{
    if( !fIM32Inited )
        return 1;

    return pfnImGetStatusString( himi, ulIndex, pBuf, pulBufLen );
}

APIRET APIENTRY ImConvertString( HIMI himi,
                                 PSZ pSrc,
                                 PCANDIDATELISTHEADER pDst,
                                 PULONG pulBufLen,
                                 ULONG ulFlag )
{
    if( !fIM32Inited )
        return 1;

    return pfnImConvertString( himi, pSrc, pDst, pulBufLen, ulFlag );
}

APIRET APIENTRY ImQueryIMMode( HIMI himi,
                               PULONG pulInputMode,
                               PULONG pulConversionMode )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMMode( himi, pulInputMode, pulConversionMode );
}

APIRET APIENTRY ImSetIMMode( HIMI himi,
                             ULONG ulInputMode,
                             ULONG ulConversionMode )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetIMMode( himi, ulInputMode, ulConversionMode );
}

APIRET APIENTRY ImQueryIMEInfo( ULONG ImeId, PIMEINFOHEADER pImeInfoHeader,
                                PULONG pulBufLen )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMEInfo( ImeId, pImeInfoHeader, pulBufLen );
}

APIRET APIENTRY ImQueryIMEProperty( HIMI himi, ULONG ulIndex, PULONG pulProp )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMEProperty( himi, ulIndex, pulProp );
}

APIRET APIENTRY ImRequestIME( HIMI himi, ULONG ulAction,
                              ULONG ulIndex, ULONG ulValue )
{
    if( !fIM32Inited )
        return 1;

    return pfnImRequestIME( himi, ulAction, ulIndex, ulValue );
}

APIRET APIENTRY ImIsIMEMessage( HWND hwndIm,
                                ULONG msg,
                                MPARAM mp1,
                                MPARAM mp2,
                                PBOOL pfResult )
{
    if( !fIM32Inited )
        return 1;

    return pfnImIsIMEMessage( hwndIm, msg, mp1, mp2, pfResult );
}

APIRET APIENTRY ImQueryIMEList( ULONG ulCodepage,
                                PULONG aImeId,
                                PULONG pulCount )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMEList( ulCodepage, aImeId, pulCount );
}

APIRET APIENTRY ImQueryCurrentIME( HIMI himi, PULONG pImeId )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryCurrentIME( himi, pImeId );
}

APIRET APIENTRY ImSetCurrentIME( HIMI himi,
                                 ULONG ulAction,
                                 ULONG ImeId )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetCurrentIME( himi, ulAction, ImeId );
}

APIRET APIENTRY ImQueryDefaultIME( ULONG ulCodepage, PULONG pImeId )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryDefaultIME( ulCodepage, pImeId );
}

APIRET APIENTRY ImSetDefaultIME( ULONG ulCodepage, ULONG ImeId )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetDefaultIME( ulCodepage, ImeId );
}


APIRET APIENTRY ImSetIMEListOrder( ULONG ulCodepage,
                                   ULONG ulAction,
                                   ULONG ImeIdTarget,
                                   ULONG ImeIdIndex )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetIMEListOrder( ulCodepage, ulAction, ImeIdTarget, ImeIdIndex );
}

APIRET APIENTRY ImQueryIMEID( PSZ pszIMEName, PULONG pImeId )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMEID( pszIMEName, pImeId );
}

APIRET APIENTRY ImQueryMsgQueueProperty( HMQ hmq, PULONG pulFlag )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryMsgQueueProperty( hmq, pulFlag );
}

APIRET APIENTRY ImSetMsgQueueProperty( HMQ hmq, ULONG ulFlag )
{
    if( !fIM32Inited )
        return 1;

    return pfnImSetMsgQueueProperty( hmq, ulFlag );
}

APIRET APIENTRY ImRequestEvent( HIMI hImi,
                                ULONG msg,
                                ULONG mp1,
                                ULONG mp2 )
{
    if( !fIM32Inited )
        return 1;

    return pfnImRequestEvent( hImi, msg, mp1, mp2 );
}


APIRET APIENTRY ImRequestIMInstance( HIMI hImi,
                                     PIMINSTANCE * ppIMInstance )
{
    if( !fIM32Inited )
        return 1;

    return pfnImRequestIMInstance( hImi, ppIMInstance );
}

APIRET APIENTRY ImReleaseIMInstance( HIMI hImi )
{
    if( !fIM32Inited )
        return 1;

    return pfnImReleaseIMInstance( hImi );
}

APIRET APIENTRY ImQueryIMInstanceReqCount( HIMI hImi,
                                           PULONG pulCount )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMInstanceReqCount( hImi, pulCount );
}

APIRET APIENTRY ImCreateIMIPart( ULONG ulPartSize,
                                 PHIMIP phimiPart )
{
    if( !fIM32Inited )
        return 1;

    return pfnImCreateIMIPart( ulPartSize, phimiPart );
}

APIRET APIENTRY ImDestroyIMIPart( HIMIP himiPart )
{
    if( !fIM32Inited )
        return 1;

    return pfnImDestroyIMIPart( himiPart );
}

APIRET APIENTRY ImQueryIMIPartSize( HIMIP himiPart,
                                    PULONG pulPartSize )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMIPartSize( himiPart, pulPartSize );
}

APIRET APIENTRY ImRequestIMIPart( HIMIP himiPart,
                                  PVOID * ppPartData )
{
    if( !fIM32Inited )
        return 1;

    return pfnImRequestIMIPart( himiPart, ppPartData );
}

APIRET APIENTRY ImReleaseIMIPart( HIMIP himiPart )
{
    if( !fIM32Inited )
        return 1;

    return pfnImReleaseIMIPart( himiPart );
}

APIRET APIENTRY ImResizeIMIPart( HIMIP himiPart,
                                 ULONG ulNewSize )
{
    if( !fIM32Inited )
        return 1;

    return pfnImResizeIMIPart( himiPart, ulNewSize );
}

APIRET APIENTRY ImQueryIMIPartReqCount( HIMIP himiPart,
                                        PULONG pulCount )
{
    if( !fIM32Inited )
        return 1;

    return pfnImQueryIMIPartReqCount( himiPart, pulCount );
}

APIRET APIENTRY ImBroadcastData( PSZ pszIMEName,
                                 ULONG ulIndex,
                                 PBROADCASTDATAHEADER pData )
{
    if( !fIM32Inited )
        return 1;

    return pfnImBroadcastData( pszIMEName, ulIndex, pData );
}

