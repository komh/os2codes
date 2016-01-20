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

#ifndef OS2CODES_SETPROCESSCP_H
#define OS2CODES_SETPROCESSCP_H

#ifdef __cplusplus
extern "C" {
#endif

#define CP_INHERIT    1
#define CP_NOINHERIT  0

int setProcessCp( int cp, int inherit );

#ifdef __cplusplus
}
#endif

#endif
