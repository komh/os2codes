/*
 * getifname(): mangle an interface name from an index and a description
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_GETIFNAME_H
#define OS2CODES_GETIFNAME_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get an interface name from an index and a description of @a struct ifmib
 *
 * @param[out] ifname Where to store an interface name.
 * @param[in] index Index of a network interface.
 * @param[in] descr Description of a network interface.
 * @return @a ifname containing an interface name.
 */
char *getifname( char *ifname, int index, const char *descr );

#ifdef __cplusplus
}
#endif

#endif
