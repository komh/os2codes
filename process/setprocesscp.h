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
 * @def cpSet Code page setting flag
 * @{ */
/** Set a current code page to a primary code page */
#define CP_SET_PRIMARY  0

/** Set a current code page to a secondary code page */
#define CP_SET_SECONDARY 1
/* @} */

/**
 * @brief Set a current code page
 * @param[in] cp New code page. A country code page or @ref cpSet flag
 * @return 0 on success, otherwise -1 with setting errno
 * @remark A child process will inherits a new code page
 */
int setProcessCp( int cp );

#ifdef __cplusplus
}
#endif

#endif
