/*
 * SIGFPE handler for OS/2
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file sigfpe.h */

#ifndef OS2CODES_SIGFPE_H
#define OS2CODES_SIGFPE_H

#include <os2.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Install SIGFPE handler
 * @param[in] pregRec A pointer to the exception registration record to be
 * registerd. This exception registration record must be on the stack
 */
void installSigfpeHandler( EXCEPTIONREGISTRATIONRECORD *pregRec );

/**
 * @brief Uninstall SIGFPE handler
 * @param[in] pregRec A pointer to the exception registration record to be
 * registerd.
 */
void uninstallSigfpeHandler( EXCEPTIONREGISTRATIONRECORD *pregRec );

#ifdef __cplusplus
}
#endif

#endif
