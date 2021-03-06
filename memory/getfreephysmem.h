/*
 * getFreePhysMem(): Get size of free physical memory
 *
 * Copyright (C) 2021 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_GETFREEPHYSMEM_H
#define OS2CODES_GETFREEPHYSMEM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get size of free physical memory
 *
 * @return Size of free physical memory
 * @return 0 on error
 */
uint32_t getFreePhysMem( void );

#ifdef __cplusplus
}
#endif

#endif
