/*
 * DBCS string functions
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_DBCSSTR_H
#define OS2CODES_DBCSSTR_H

#ifdef __cplusplus
extern "C" {
#endif

int dbcsIsEnabled( void );
int dbcsStrlen( const char *str );
int dbcsIsLeadByte( const char *str, int pos );
int dbcsIsTrailByte( const char *str, int pos );

#ifdef __cplusplus
}
#endif

#endif
