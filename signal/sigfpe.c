/*
 * SIGFPE handler for OS/2
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This is excerted from nsprpub/pr/src/md/os2/os2thred.c of Mozilla project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define INCL_DOS
#include <os2.h>

#include <float.h>
#include <string.h>

#include "sigfpe.h"

static
ULONG _System sigfpeHandler( PEXCEPTIONREPORTRECORD p1,
                             PEXCEPTIONREGISTRATIONRECORD p2,
                             PCONTEXTRECORD p3,
                             PVOID p4 )
{
    switch( p1->ExceptionNum )
    {
        case XCPT_FLOAT_DENORMAL_OPERAND:
        case XCPT_FLOAT_DIVIDE_BY_ZERO:
        case XCPT_FLOAT_INEXACT_RESULT:
        case XCPT_FLOAT_INVALID_OPERATION:
        case XCPT_FLOAT_OVERFLOW:
        case XCPT_FLOAT_STACK_CHECK:
        case XCPT_FLOAT_UNDERFLOW:
        {
            unsigned cw = p3->ctx_env[0];
            if ((cw & MCW_EM) != MCW_EM) {
                /* Mask out all floating point exceptions */
                p3->ctx_env[0] |= MCW_EM;
                /* Following two lines set precision to 53 bit mantissa. */
                p3->ctx_env[0] &= ~MCW_PC;
                p3->ctx_env[0] |= PC_53;

                return XCPT_CONTINUE_EXECUTION;
            }

            break;
        }
    }

    return XCPT_CONTINUE_SEARCH;
}

void installSigfpeHandler( EXCEPTIONREGISTRATIONRECORD *pregRec )
{
    memset( pregRec, 0, sizeof( *pregRec ));

    pregRec->ExceptionHandler = ( ERR )sigfpeHandler;
    DosSetExceptionHandler( pregRec );
}

void uninstallSigfpeHandler( EXCEPTIONREGISTRATIONRECORD *pregRec )
{
    DosUnsetExceptionHandler( pregRec );
}

#include <stdio.h>
#include <float.h>

int main(void)
{
    EXCEPTIONREGISTRATIONRECORD regRec;
    float f, f2;

    // mask on all exceptions
    _control87( 0, MCW_EM );

    installSigfpeHandler( &regRec );

    f2 = 1.5;
    f = .0;
    f = f2 / f;
    printf("f = %f\n", f );

    uninstallSigfpeHandler( &regRec );

    return 0;
}
