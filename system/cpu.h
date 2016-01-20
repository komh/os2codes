/*
 * CPU functions
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file cpu.h */

#ifndef OS2CODES_CPU_H
#define OS2CODES_CPU_H

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief structure for CPUTIME
 */
typedef struct tagCPUTIME
{
    uint64_t time;  /**< total time snapshot */
    uint64_t idle;  /**< idle time snapshot */
    uint64_t busy;  /**< busy time snapshot */
    uint64_t intr;  /**< interrupt time snapshot */
} CPUTIME, *PCPUTIME;

/**
 * @brief Get a number of CPUs
 * @return CPU count
 */
int getCpuCount( void );

/**
 * @brief Get a number of CPUs which are online
 * @return A number of CPUs which are online
 */
int getCpuOnCount( void );

/**
 * @brief Get a CPU time snapshots
 * @param[out] pCpuTime Place to store CPU time snapshots. This should be
 * enough to store time snapshots of All CPUs. So you must get a cpu count
 * with #getCpuCount() before.
 * @param[in] size Size of the buffer pointed by @a pCpuTime in bytes.
 * @return 0 on success, -1 on error with setting errno
 */
int getCpuTime( PCPUTIME pCpuTime, size_t size );

#ifdef __cplusplus
}
#endif
#endif
