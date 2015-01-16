/*
 * OS/2 32 bit Input Method dynamic loader test program
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#include <os2.h>

#include <os2im.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "im32.h"

#define WC_IM32TEST "IM32 Test"

#define ID_IM32TEST 1

typedef struct IM32DATA
{
    char  szConv[ 256 ];     /**< Conversion string. Assume 256 is maximum */
    char  szResult[ 256 ];   /**< Result string. Assume 256 is maximum */
    int   selStart;          /**< Start position of CP_ATTR_TARGET_CONVERTED */
    int   selLength;         /**< Length of CP_ATTR_TARGET_CONVERTED */
    int   curPosConv;        /**< Cursor position in a conversion string */
    int   curPos;            /**< Cursor position in a line buffer */
} IM32DATA, *PIM32DATA;

/**
 * Process WM_CREATE
 *
 * @return TRUE  to discontinue window creation,
 *         FALSE to continue window creation
 */
static MRESULT WmCreate( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    PIM32DATA pIm32Data;

    pIm32Data = calloc( 1, sizeof( *pIm32Data ));
    if( pIm32Data )
    {
        WinSetWindowPtr( hwnd, 0, pIm32Data );

        return FALSE;
    }

    return ( MRESULT )TRUE;
}

/**
 * Process WM_DESTROY
 *
 * @return 0
 */
static MRESULT WmDestroy( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    free( pIm32Data );

    return 0;
}

/**
 * Determine if @a ch is a DBCS lead byte
 *
 * @return TRUE if a DBCS lead byte, otherwise FALSE
 */
static BOOL IsDBCSLeadByte( char ch )
{
    COUNTRYCODE cc;
    CHAR coderange[ 12 ];

    cc.country = 0;
    cc.codepage = 0;

    if( DosQueryDBCSEnv( sizeof( coderange ), &cc, coderange) == 0 )
    {
        int i;

        for( i = 0; coderange[ i ] || coderange[ i + 1 ]; i += 2 )
        {
            if( coderange[ i ] <= ch && ch <= coderange[ i + 1 ])
                return TRUE;
        }
   }

   return FALSE;
}

/**
 * Determine if a character at position @a index of a string pointed by @a s
 * is a DBCS trail byte
 *
 * @return TRUE if a DBCS trail byte, otherwise FALSE
 */
static BOOL IsDBCSTrailByte( const char *s, int index )
{
    int i;

    for( i = 0; i < index; i++ )
    {
        if( IsDBCSLeadByte( s[ i ]))
            i++;
    }

    return ( i > index ) ? TRUE : FALSE;
}

/**
 * Get a result string from IME
 *
 * @return TRUE on success, or FALSE on error
 */
static MRESULT GetResultString( HWND hwnd )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    HIMI  himi;
    PCHAR pStrBuf = NULL;
    ULONG ulStrBufLen;
    int   i;
    BOOL  rc = FALSE;

    if( ImGetInstance( hwnd , &himi ))
        return FALSE;

    ulStrBufLen = 0L; /* set 0, in order to inquery the string size */
    if( ImGetResultString( himi, IMR_RESULT_RESULTSTRING,
                           pStrBuf, &ulStrBufLen ))
        goto exit_free;

    /* Note that ulStrBufLen does not include null-terminator */
    pStrBuf = malloc( ulStrBufLen );
    if( ImGetResultString( himi, IMR_RESULT_RESULTSTRING,
                           pStrBuf, &ulStrBufLen ))
        goto exit_free;

    pIm32Data->szConv[ 0 ] = '\0';   /* Clear a current conversion string */

    /* Let WM_CHAR process a result string */
    for( i = 0; i < ulStrBufLen; i++ )
    {
        if( IsDBCSLeadByte( pStrBuf[ i ] ))
        {
            WinSendMsg( hwnd , WM_CHAR ,
                        MPFROMSH2CH( KC_CHAR, 0, 0 ),
                        MPFROM2SHORT( MAKEUSHORT(( UCHAR )pStrBuf[ i ],
                                                 ( UCHAR )pStrBuf[ i + 1 ]),
                                      0 ));

            i++;    /* Skip a trail byte */
        } else {
            WinSendMsg( hwnd , WM_CHAR ,
                        MPFROMSH2CH( KC_CHAR, 0, 0 ),
                        MPFROM2SHORT( MAKEUSHORT(( UCHAR )pStrBuf[ i ], 0 ),
                                      0 ));
        }
    }

    rc = TRUE;

exit_free:

    free( pStrBuf );

    ImReleaseInstance( hwnd, himi );

    return ( MRESULT )rc;
}

/**
 * Get a conversion string from IME
 *
 * @return TRUE on success, or FALSE on error
 */
static MRESULT GetConversionString( HWND hwnd )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    HIMI  himi;
    PCHAR pStrBuf = NULL;
    ULONG ulBufLen;
    PCHAR pAttrBuf = NULL;
    int   selStart;
    int   selEnd;
    ULONG ulCursorPos;
    int   i;
    BOOL  rc = FALSE;

    rc = ImGetInstance ( hwnd , &himi );
    if( rc )
        return FALSE;

    /* Get a conversion string */
    ulBufLen = 0L; /* set 0, in order to inquery the string size */
    if( ImGetConversionString( himi, IMR_CONV_CONVERSIONSTRING,
                               pStrBuf, &ulBufLen ))
        goto exit_free;

    /* Note that ulBufLen does not include null-terminator */
    pStrBuf = calloc( 1, ulBufLen + 1 ); /* 1 for null-terminator */
    if( ImGetConversionString( himi, IMR_CONV_CONVERSIONSTRING,
                               pStrBuf, &ulBufLen ))
        goto exit_free;

    strncpy( pIm32Data->szConv, pStrBuf, sizeof( pIm32Data->szConv ));

    /* Ensure null-terminator */
    pIm32Data->szConv[ sizeof( pIm32Data->szConv ) - 1 ] = '\0';

    /* Get an attribute of a conversion string */
    ulBufLen = 0L; /* set 0, in order to inquery the attr buffer size */
    if( !ImGetConversionString( himi, IMR_CONV_CONVERSIONATTR,
                                pAttrBuf, &ulBufLen ))
    {
        pAttrBuf = malloc( ulBufLen );
        if( ImGetConversionString( himi, IMR_CONV_CONVERSIONATTR,
                                   pAttrBuf, &ulBufLen ))
            ulBufLen = 0;   /* process errors silently */
    }

    selStart = ulBufLen + 1;    /* Set to maximum,
                                   1 for the case ulBufLen == 0 */
    selEnd   = -1;              /* Set to minimum */

    /* Find CP_ATTR_TARGET_CONVERTED. It occurs only once */
    for( i = 0; i < ulBufLen; i++ )
    {
        if( pAttrBuf[ i ] == CP_ATTR_TARGET_CONVERTED )
        {
            if( selStart > i )
                selStart = i;

            if( selEnd < i )
                selEnd = i;
        }
    }

    pIm32Data->selStart  = selStart;
    pIm32Data->selLength = ( selEnd + 1 ) - selStart;
    if( pIm32Data->selLength <= 0 )
    {
        pIm32Data->selStart  = 0;
        pIm32Data->selLength = 0;
    }

    /* Get a cursor position in the conversion string */
    ulBufLen = sizeof( ulCursorPos );
    if( ImGetConversionString( himi, IMR_CONV_CURSORPOS,
                               &ulCursorPos, &ulBufLen ))
        ulCursorPos = 0;    /* process error silently */

    pIm32Data->curPosConv = ulCursorPos;

    /* Update window */
    WinInvalidateRect( hwnd, NULL, TRUE );

    rc = TRUE;

exit_free :

    free( pStrBuf );
    free( pAttrBuf );

    return ( MRESULT )rc;
}

/**
 * Process WM_IMEREQUEST
 *
 * @return TRUE on success, or FALSE on error
 */
static MRESULT WmImeRequest( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    /* If IM32 was not initailzed, fall back to a default window procedure
       in order to support WM_QUERYCONVERTPOS based input methods */
    if( im32Inited())
    {
        if( LONGFROMMP( mp1 ) == IMR_CONVRESULT )
        {
            if( LONGFROMMP( mp2 ) & IMR_RESULT_RESULTSTRING )
                return GetResultString( hwnd );
            else if( LONGFROMMP( mp2 ) & IMR_CONV_CONVERSIONSTRING )
                return GetConversionString( hwnd );

            /* Do not pass to a default window procedure. Otherwise, A default
               conversion window of IME itself shows up at the same time */
            return FALSE;   /* Not processed */
        }

        /* Pass other messages to a default window procedure. Especially,
           IMR_INSTANCEACTIVATE and IMR_CANDIDATE let IME process a candidate part */
    }

    return WinDefWindowProc( hwnd, WM_IMEREQUEST, mp1, mp2 );
}

/**
 * Process KC_VIRTUALKEY flags
 *
 */
static void KcVirtualKey( HWND hwnd, ULONG ulVk )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    int chSize;

    switch( ulVk )
    {
        case VK_SPACE :
            WinSendMsg( hwnd, WM_CHAR,
                        MPFROMSH2CH( KC_CHAR, 0, 0 ), MPFROM2SHORT(' ', 0 ));
            break;

        case VK_DELETE :
            if( pIm32Data->szResult[ pIm32Data->curPos ])
            {
                chSize = IsDBCSLeadByte(
                            pIm32Data->szResult[ pIm32Data->curPos ] )
                         ? 2 : 1;
                memmove( pIm32Data->szResult + pIm32Data->curPos,
                         pIm32Data->szResult + pIm32Data->curPos + chSize,
                         strlen( pIm32Data->szResult )
                            - pIm32Data->curPos - chSize + 1 );
            }
            break;

        case VK_BACKSPACE :
            if( pIm32Data->curPos > 0 )
            {
                chSize = IsDBCSTrailByte(
                            pIm32Data->szResult, pIm32Data->curPos - 1 )
                         ? 2 : 1;

                pIm32Data->curPos -= chSize;
                memmove( pIm32Data->szResult + pIm32Data->curPos,
                         pIm32Data->szResult + pIm32Data->curPos + chSize,
                         strlen( pIm32Data->szResult ) - pIm32Data->curPos
                            - chSize + 1 );
            }
            break;

        case VK_LEFT :
            if( pIm32Data->curPos > 0 )
            {
                pIm32Data->curPos -= IsDBCSTrailByte( pIm32Data->szResult,
                                                      pIm32Data->curPos - 1 )
                                     ? 2 : 1;
            }
            break;

        case VK_RIGHT :
            if( pIm32Data->curPos < strlen( pIm32Data->szResult ))
            {
                pIm32Data->curPos +=
                    IsDBCSLeadByte( pIm32Data->szResult[ pIm32Data->curPos ])
                    ? 2 : 1;
            }
            break;
    }
}

/**
 * Process KC_CHAR flag
 */
static void KcChar( HWND hwnd, ULONG ulCh )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    int len = strlen( pIm32Data->szResult );

    memmove( pIm32Data->szResult + pIm32Data->curPos
                + ( HIBYTE( ulCh ) /* DBCS ? */ ? 2 : 1 ),
             pIm32Data->szResult + pIm32Data->curPos,
             len - pIm32Data->curPos + 1 );

    pIm32Data->szResult[ pIm32Data->curPos++ ] = LOBYTE( ulCh );
    if( HIBYTE( ulCh )) /* DBCS ? */
        pIm32Data->szResult[ pIm32Data->curPos++ ] = HIBYTE( ulCh );
}

/**
 * Process WM_CHAR
 *
 * @return TRUE on processed, or FALSE on ignored
 */
static MRESULT WmChar( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    if( SHORT1FROMMP( mp1 ) & KC_KEYUP )
        return WinDefWindowProc( hwnd, WM_CHAR, mp1, mp2 );

    if( SHORT1FROMMP( mp1 ) & KC_VIRTUALKEY )
        KcVirtualKey( hwnd, SHORT2FROMMP( mp2 ));
    else if( SHORT1FROMMP( mp1 ) & KC_CHAR )
        KcChar( hwnd, SHORT1FROMMP( mp2 ));

    /* Update window */
    WinInvalidateRect( hwnd , NULL, TRUE );

    return ( MRESULT )TRUE;
}

/**
 * Calculate a width in pixels of a string pointed by @a str, up to count
 * @a count
 *
 * @return a width in pixels
 */
static LONG CalcStrWidth( HWND hwnd, const char *str, int count )
{
    HPS hps;
    POINTL aptl[ TXTBOX_COUNT ];

    hps = WinGetPS( hwnd );

    GpiQueryTextBox( hps, count, ( PCH )str, TXTBOX_COUNT, aptl );

    WinReleasePS( hps );

    return ( aptl[ TXTBOX_CONCAT ].x - aptl[ TXTBOX_BOTTOMLEFT ].x );
}

/**
 * Calculate X position in pixels for a cursor
 *
 * @return X position in pixels
 */
static LONG CalcXCurPos( HWND hwnd )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    LONG lPos = CalcStrWidth( hwnd, pIm32Data->szResult, pIm32Data->curPos );

    if( pIm32Data->szConv[ 0 ])
        lPos += CalcStrWidth( hwnd, pIm32Data->szConv, pIm32Data->curPosConv );

    return lPos;
}

/**
 * Calculate a height of a character in pixels
 *
 * @return a height of a chracter in pixels
 */
static LONG CalcCharHeight( HWND hwnd )
{
    HPS hps;
    FONTMETRICS fm;

    hps = WinGetPS( hwnd );

    GpiQueryFontMetrics( hps, sizeof( fm ), &fm );

    WinReleasePS( hps );

    /* Maximum height of a font */
    return ( fm.lMaxBaselineExt + fm.lExternalLeading );
}

/**
 * Calculate Y position in pixels corresponding to a line @a line
 *
 * @param[in] line 0-based line number from a top of a window

 * @return Y position in pixels
 */
static LONG CalcYPos( HWND hwnd, int line )
{
    HPS hps;
    RECTL rcl;
    FONTMETRICS fm;

    hps = WinGetPS( hwnd );

    WinQueryWindowRect( hwnd, &rcl );
    GpiQueryFontMetrics( hps, sizeof( fm ), &fm );

    WinReleasePS( hps );

    line++;

    return ( rcl.yTop - CalcCharHeight( hwnd ) * line + fm.lMaxDescender );
}

/**
 * Calculate Y position in pixels corresponding to a line @a line for a cursor
 *
 * @param[in] line 0-based line number from a top of a window
 *
 * @return Y position in pixels
 */
static LONG CalcYCurPos( HWND hwnd, int line )
{
    HPS hps;
    RECTL rcl;
    FONTMETRICS fm;

    hps = WinGetPS( hwnd );

    WinQueryWindowRect( hwnd, &rcl );
    GpiQueryFontMetrics( hps, sizeof( fm ), &fm );

    WinReleasePS( hps );

    line++;

    return ( rcl.yTop - CalcCharHeight( hwnd ) * line );
}

#define LINE_LABEL  0
#define LINE_RESULT 1

/**
 * Process WM_PAINT
 *
 * @return 0
 */
static MRESULT WmPaint( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    PIM32DATA pIm32Data = WinQueryWindowPtr( hwnd, 0 );

    HPS hps;
    RECTL rcl;

    POINTL ptl;
    POINTL ptlConv;
    LONG   lOldLineType;

    hps = WinBeginPaint( hwnd, NULLHANDLE, &rcl);

    /* Clear invalidated area */
    WinFillRect( hps, &rcl, SYSCLR_WINDOW );

    WinQueryWindowRect( hwnd, &rcl );

    /* Overwrite a background */
    GpiSetBackMix( hps, BM_OVERPAINT );

    /* Dispaly a conversion string at middle of a window */
    ptl.x = 0;
    ptl.y = rcl.yTop / 2;
    GpiCharStringAt( hps, &ptl, 20, "Conversion string : " );
    GpiCharString( hps, strlen( pIm32Data->szConv ), pIm32Data->szConv );

    /* Display a label for a result string at first line */
    ptl.x = 0;
    ptl.y = CalcYPos( hwnd, LINE_LABEL );
    GpiCharStringAt( hps, &ptl, 16, "Result string : " );

    /* Move a current position to a second line */
    ptl.x = 0;
    ptl.y = CalcYPos( hwnd, LINE_RESULT );
    GpiMove( hps, &ptl );

    /* Display a result string before a conversion string */
    GpiCharString( hps, pIm32Data->curPos, pIm32Data->szResult );

    /* Conversion string ? */
    if( pIm32Data->szConv[ 0 ])
    {
        /* Save a current position for a conversion string */
        GpiQueryCurrentPosition( hps, &ptlConv );

        /* Display a conversion string before a selection part */
        GpiCharString( hps, pIm32Data->selStart, pIm32Data->szConv );

        /* Invert colors for a selection part */
        GpiSetColor( hps, SYSCLR_WINDOW );
        GpiSetBackColor( hps, SYSCLR_WINDOWTEXT );

        /* Display a conversion string in a selection part */
        GpiCharString( hps, pIm32Data->selLength,
                       pIm32Data->szConv + pIm32Data->selStart );

        /* Restore colors */
        GpiSetColor( hps, SYSCLR_WINDOWTEXT );
        GpiSetBackColor( hps, SYSCLR_WINDOW );

        /* Display a conversion string after a selection part */
        GpiCharString( hps,
                       strlen( pIm32Data->szConv ) - pIm32Data->selStart
                            - pIm32Data->selLength,
                       pIm32Data->szConv + pIm32Data->selStart
                            + pIm32Data->selLength );

        /* Save a current position for a remaining result string */
        GpiQueryCurrentPosition( hps, &ptl );

        /* Move to the position to draw a under line for a conversion string */
        ptlConv.y = CalcYCurPos( hwnd, LINE_RESULT );
        GpiMove( hps, &ptlConv );

        /* Draw a under line to mark a conversion string */
        ptlConv.x += CalcStrWidth( hwnd, pIm32Data->szConv,
                                   strlen( pIm32Data->szConv )) * 2;
        lOldLineType = GpiQueryLineType( hps );
        GpiSetLineType( hps, LINETYPE_DOT );
        GpiLine( hps, &ptlConv );

        /* Restore a position for a remaining result string */
        GpiMove( hps, &ptl );
    }

    /* Display a remaining result string */
    GpiCharString( hps, strlen( pIm32Data->szResult ) - pIm32Data->curPos,
                   pIm32Data->szResult + pIm32Data->curPos );

    /* Remove trailing parts */
    GpiQueryCurrentPosition( hps, &ptl );
    rcl.xLeft = ptl.x;
    rcl.yBottom = CalcYCurPos( hwnd, LINE_RESULT );
    rcl.yTop = rcl.yBottom + CalcCharHeight( hwnd );
    WinFillRect( hps, &rcl, SYSCLR_WINDOW );

    /* Show a cursor */
    if( WinQueryFocus( HWND_DESKTOP ) == hwnd )
    {
        ptl.x = CalcXCurPos( hwnd );
        ptl.y = CalcYCurPos( hwnd, LINE_RESULT );
        WinCreateCursor( hwnd, ptl.x, ptl.y, 0, CalcCharHeight( hwnd ),
                         CURSOR_FLASH, NULL );
    }

    WinEndPaint( hps );

    return 0;
}

static MRESULT WmQueryConvertPos( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    PRECTL pcp = PVOIDFROMMP( mp1 );

    pcp->xLeft   = CalcXCurPos( hwnd );
    pcp->yBottom = CalcYCurPos( hwnd, LINE_RESULT );

    /* In fact, both xRight and yTop seem to be ignored. However, set them
       to proper values. And assume that a width of a DBCS character is
       twice wider than a SBCS character */
    pcp->xRight  = pcp->xLeft + CalcStrWidth( hwnd, "W", 1 ) * 2;
    pcp->yTop    = pcp->yBottom + CalcCharHeight( hwnd );

    return ( MRESULT )QCP_CONVERT;
}

/**
 * Request IME to complete a conversion
 */
static void CompleteConversion( HWND hwnd )
{
    HIMI himi;

    if( ImGetInstance( hwnd, &himi ))
        return;

    ImRequestIME( himi, REQ_CONVERSIONSTRING, CNV_COMPLETE, 0 );

    ImReleaseInstance( hwnd, himi );
}

/**
 * Process WM_SETFOCUS
 *
 * @return 0
 */
static MRESULT WmSetFocus( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    WinShowCursor( hwnd, SHORT1FROMMP( mp2 ));

    return 0;
}

/**
 * Process WM_BUTTON1DOWN
 *
 * @return TRUE if processed, otherwise FALSE
 */
static MRESULT WmButton1Down( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    CompleteConversion( hwnd );

    /* Fall back to a default window procedure */
    return WinDefWindowProc( hwnd, WM_BUTTON1DOWN, mp1, mp2 );
}

/**
 * Process WM_BUTTON1DOWN
 *
 * @return TRUE if processed, otherwise FALSE
 */
static MRESULT WmButton2Down( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    CompleteConversion( hwnd );

    /* Fall back to a default window procedure */
    return WinDefWindowProc( hwnd, WM_BUTTON2DOWN, mp1, mp2 );
}

/**
 * Process WM_BUTTON1DOWN
 *
 * @return TRUE if processed, otherwise FALSE
 */
static MRESULT WmButton3Down( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    CompleteConversion( hwnd );

    /* Fall back to a default window procedure */
    return WinDefWindowProc( hwnd, WM_BUTTON3DOWN, mp1, mp2 );
}

/**
 * Window procedure for a client window
 *
 * @return in according to a message @a msg
 */
static MRESULT EXPENTRY WindowProc( HWND hwnd, ULONG msg,
                                    MPARAM mp1, MPARAM mp2 )
{
    switch( msg )
    {
        case WM_CREATE:
            return WmCreate( hwnd, mp1, mp2 );

        case WM_DESTROY:
            return WmDestroy( hwnd, mp1, mp2 );

        case WM_IMEREQUEST :
            return WmImeRequest( hwnd, mp1, mp2 );

        case WM_CHAR:
            return WmChar( hwnd, mp1, mp2 );

        case WM_PAINT:
            return WmPaint( hwnd, mp1, mp2 );

        case WM_QUERYCONVERTPOS:
            return WmQueryConvertPos( hwnd, mp1, mp2 );

        case WM_SETFOCUS:
            return WmSetFocus( hwnd, mp1, mp2 );

        case WM_BUTTON1DOWN:
            return WmButton1Down( hwnd, mp1, mp2 );

        case WM_BUTTON2DOWN:
            return WmButton2Down( hwnd, mp1, mp2 );

        case WM_BUTTON3DOWN:
            return WmButton3Down( hwnd, mp1, mp2 );
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

int main( void )
{
    HAB hab;
    HMQ hmq;
    ULONG flFrameFlags;
    HWND hwndFrame;
    HWND hwndClient;
    QMSG qm;

    PPIB ppib;

    DosGetInfoBlocks( NULL, &ppib );
    ppib->pib_ultype = 3;   /* Enable PM APIs */

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0 );

    WinRegisterClass( hab, WC_IM32TEST, WindowProc, CS_SIZEREDRAW,
                      sizeof( PVOID ));

    flFrameFlags = FCF_SYSMENU | FCF_TITLEBAR | FCF_MINMAX |
                   FCF_SIZEBORDER | FCF_TASKLIST | FCF_SHELLPOSITION;

    hwndFrame = WinCreateStdWindow(
                    HWND_DESKTOP,               /* parent window handle */
                    WS_VISIBLE,                 /* frame window style */
                    &flFrameFlags,              /* window style */
                    WC_IM32TEST,                /* class name */
                    "IM32 Test",                /* window title */
                    0L,                         /* default client style */
                    NULLHANDLE,                 /* resource in exe file */
                    ID_IM32TEST,                /* frame window id */
                    &hwndClient                 /* client window handle */
                );

    if( hwndFrame != NULLHANDLE )
    {
        /* Activate a frame window */
        WinSetWindowPos( hwndFrame, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE );

        while( WinGetMsg( hab, &qm, NULLHANDLE, 0, 0 ))
            WinDispatchMsg( hab, &qm );

        WinDestroyWindow( hwndFrame );
    }

    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    return 0;
}

