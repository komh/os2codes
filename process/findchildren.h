/*
 * findchildren(): Find PIDs of children
 *
 * Copyright (C) 2024 KO Myung-Hun <komh78@gmail.com>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_FINDCHILDREN_H
#define OS2CODES_FINDCHILDREN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

/**
 * Find PIDs of children of the given PID @a pid.
 *
 * @param[in] pid PID
 * @param[out] children Where to store PIDs of children
 * @param[in] maxchildren The maximum PID count which @a children can contains
 *            If @a maxchildren is 0, @a children is ignored.
 * @return The number of PIDs of children on success
 * @return -1 on error with setting errno
 */
int findchildren( pid_t pid, pid_t children[], size_t maxchildren );

#ifdef __cplusplus
}
#endif

#endif
