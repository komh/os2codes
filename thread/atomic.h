/*
 * Atomic functions
 *
 * Copyright (C) 2021 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

/** @file atomic.h */

#ifndef OS2CODES_ATOMIC_H
#define OS2CODES_ATOMIC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Atomic load
 *
 * @param[in] p Pointer to the place where is loaded atomically
 * @return Atomically loaded value of the place pointed by @a p
 */
int _System atomicLoad( volatile int *p );

/**
 * Atomic exchange
 *
 * @param[out] p Pointer to the place where to store a new value @a v
 * @param[in] v New value
 * @return Old value pointed by @a p
 */
int _System atomicExchange( volatile int *p, int v );

/**
 * Atomic compare and exchange
 *
 * @param[in, out] p Pointer to the place where to compare to @a e
 *                   and to exchange with @a d if the value pointed by @a p
                     and @a e are same
 * @param[in] d Desired value
 * @param[in] e Expected value
 * @return 1 if the value pointed by @a p and @a e are same, otherwise 0
 */
int _System atomicCompareExchange( volatile int *p, int d, int e );

#ifdef __cplusplus
}
#endif

#endif
