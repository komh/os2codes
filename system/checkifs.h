/*
 * checkIfs() : check if IFS was loaded
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_CHECKIFS_H
#define OS2CODES_CHECKIFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Check if the given IFS was loaded
 *
 * @param[in] fsdName FSD name
 * @return Non-zero if loaded, 0 if not loaded
 * @remark FSD name is the file-system driver name exported by the file-system
 *         driver. This name may be different from the filename of the
 *         file-system driver and/or the file-system name in the boot sector.
 */
int checkIfs( const char *fsdName );

#ifdef __cplusplus
}
#endif

#endif
