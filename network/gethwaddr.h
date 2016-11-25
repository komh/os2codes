/*
 * gethwaddr(): retrive MAC address of a network interface
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_GETHWADDR_H
#define OS2CODES_GETHWADDR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char HWADDR[ 6 ];

/**
 * Get MAC address of a network interface
 *
 * @param[in] ifname name of a network interface.
 * @param[out] hwaddr where to store MAC address.
 * @return 0 on success, or -1 on error.
 */
int gethwaddr( const char *ifname, HWADDR hwaddr );

#ifdef __cplusplus
}
#endif

#endif
