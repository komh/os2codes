/*
 * FIFO functions
 *
 * Copyright (C) 2024 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_FIFO_H
#define OS2CODES_FIFO_H

/** FIFO handle */
typedef struct fifo *HFIFO;

/**
 * @defgroup fifoflags FIFO flags
 * @{
 */
#define FIFO_BLOCKING       0x0001
#define FIFO_EXPANDABLE     0x0002
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a FIFO
 *
 * @param[in] capacity Capacity of FIFO in elements
 * @param[in] elementsize Size of element
 * @param[in] flags Flags for FIFO
 * @return FIFO handle on success, or NULL on error with setting errno
 */
HFIFO fifoCreate( size_t capacity, size_t elementsize, int flags );

/**
 * Destroy a FIFO
 *
 * @param fifo FIFO handle
 * @return 0 on success, -1 on error with setting errno
 */
int fifoDestroy( HFIFO fifo );

/**
 * Read from a FIFO
 *
 * @param fifo FIFO handle
 * @param[out] buffer Buffer to read from a FIFO to
 * @param[in] elements The number of elemenets to read
 * @return The number of elements read on success
 * @return -1 on error with setting errno
 * @remark If FIFO_BLOCKING is set, fifoRead() waits until reading up to
 *         elements specified by @a elements.
 */
int fifoRead( HFIFO fifo, void *buffer, size_t elements );

/**
 * Write to a FIFO
 *
 * @param fifo FIFO handle
 * @param[in] buffer Buffer to write to FIFO
 * @param[in] elements The number of elements to write
 * @return The number of elements written on success
 * @return -1 on error with setting errno
 * @remark If FIFO_BLOCKING is set, fifoWrite() waits until writing up to
 *         elements specified by @a elements.
 * @remark If FIFO_EXPANDABLE is set, FIFO expands automatically if full.
 */
int fifoWrite( HFIFO fifo, const void *buffer, size_t elements );

/**
 * Clear a buffer of a FIFO
 *
 * @param fifo FIFO handle
 * @return 0 on success, -1 on error with setting errno
 */
int fifoClear( HFIFO fifo );

/**
 * Shrink a FIFO buffer to fit elements in a FIFO
 *
 * @param fifo FIFO handle
 * @return Capacity of a shrinked FIFO in elements on success
 * @return -1 on error with setting errno
 */
int fifoShrink( HFIFO fifo );

/**
 * Get capacity of a FIFO
 *
 * @param[in] fifo FIFO handle
 * @return Capacity of a FIFO in elements
 */
size_t fifoCapacity( const HFIFO fifo );

/**
 * Get the number of elements in a FIFO
 *
 * @param[in] fifo FIFO handle
 * @return The number of elemenets in a FIFO
 */
size_t fifoElements( const HFIFO fifo );

/**
 * Check if FIFO is empty
 *
 * @param[in] f FIFO handle
 * @return Non-zero if empty, or 0
 */
#define fifoEmpty( f )  ( fifoElements(( f )) == 0 )

/**
 * Get spaces in a FIFO
 *
 * @param[in] fifo FIFO handle
 * @return Spaces of a FIFO in elements
 */
size_t fifoSpaces( const HFIFO fifo );

/**
 * Check if FIFO is full
 *
 * @param[in] f FIFO handle
 * @return Non-zero if full, or 0
 */
#define fifoFull( f )   ( fifoSpaces(( f )) == 0 )

#ifdef __cplusplus
}
#endif

#endif
