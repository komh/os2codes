/*
 * pipe() supporting a non-blocking mode using a named pipe for OS/2
 *
 * Copyright (C) 2014-2021 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_NAMEDPIPE_H
#define OS2CODES_NAMEDPIPE_H

#ifdef __cplusplus
extern "C" {
#endif

int namedpipe( int *ph );

#ifdef __cplusplus
}
#endif

#endif
