/*
 * functions to set a code page
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file setprocesscp.h */

#ifndef OS2CODES_SETPROCESSCP_H
#define OS2CODES_SETPROCESSCP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def cpInherit Code page inheritance flag
 * @{ */
/** Make sure that a current code page is inherited by a child process */
#define CP_INHERIT    1

/**
 * Let a system determine whether or not a current code page is inherited by
 * a child process
 */
#define CP_NOINHERIT  0
/* @} */

/**
 * @brief Set a current code page
 * @param[in] cp New code page
 * @param[in] inherit A code page inheritance flag. See @ref cpInherit
 * @return 0 on success, otherwise -1 with setting errno
 */
int setProcessCp( int cp, int inherit );

#ifdef __cplusplus
}
#endif

#endif
