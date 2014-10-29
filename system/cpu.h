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

#ifndef OS2CODES_CPU_H
#define OS2CODES_CPU_H

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagCPUTIME
{
    uint64_t time;
    uint64_t idle;
    uint64_t busy;
    uint64_t intr;
} CPUTIME, *PCPUTIME;

int getCpuCount( void );
int getCpuOnCount( void );
int getCpuTime( PCPUTIME pCpuTime, size_t size );

#ifdef __cplusplus
}
#endif
#endif
