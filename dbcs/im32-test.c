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

#define INCL_WIN
#define INCL_DOS
#include <os2.h>

#include <os2im.h>

#include <stdio.h>
#include <string.h>

#define WC_IM32TEST "IM32 Test"

#define ID_IM32TEST 1

MRESULT EXPENTRY WindowProc( HWND, ULONG, MPARAM, MPARAM );

int main( void )
{
    PPIB  ppib;
    HAB   hab;
    HMQ   hmq;
    ULONG flFrameFlags;
    HWND  hwndFrame;
    HWND  hwndClient;
    QMSG  qm;

    DosGetInfoBlocks( NULL, &ppib );
    ppib->pib_ultype = 3;

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0 );

    WinRegisterClass(
        hab,
        WC_IM32TEST,
        WindowProc,
        CS_SIZEREDRAW,
        sizeof( PVOID )
    );

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
        WinSetWindowPos( hwndFrame, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE );

        while( WinGetMsg( hab, &qm, NULLHANDLE, 0, 0 ))
            WinDispatchMsg( hab, &qm );

        WinDestroyWindow( hwndFrame );
    }

    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    return 0;
}

MRESULT EXPENTRY WindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    switch( msg )
    {
        case WM_PAINT:
        {
            HPS   hps;
            RECTL rcl;
            CHAR  szMsg[ 512 ];
            HIMI  himi;
            ULONG ulInputMode;
            ULONG ulConversionMode;

            hps = WinBeginPaint( hwnd, NULLHANDLE, &rcl );

            WinFillRect( hps, &rcl, 0 );

            WinQueryWindowRect( hwnd, &rcl );

            ImGetInstance( hwnd, &himi );

            if( ImQueryIMMode( himi, &ulInputMode, &ulConversionMode ))
                strcpy( szMsg, "ImQueryIMMode() failed!!!");
            else
                snprintf( szMsg, sizeof( szMsg ),
                          "CM = %lx, Automatic : %s, Predict : %s, "
                          "Plural : %s, Single : %s",
                          ulConversionMode,
                          ulConversionMode & IMI_CM_AUTOMATIC ? "On" : "Off",
                          ulConversionMode & IMI_CM_PREDICT ? "On" : "Off",
                          ulConversionMode & IMI_CM_PLURALCLAUSE ? "On" : "Off",
                          ulConversionMode & IMI_CM_SINGLE ? "On" : "Off");

            ImReleaseInstance( hwnd, himi );

            WinDrawText( hps, strlen( szMsg ), szMsg, &rcl, 0, 0,
                         DT_CENTER | DT_VCENTER | DT_TEXTATTRS );

            WinEndPaint( hps );

            return 0;
        }
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

