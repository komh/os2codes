/*
 * SIGFPE handler test program
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

#include <stdio.h>
#include <float.h>

#include "sigfpe.h"

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
