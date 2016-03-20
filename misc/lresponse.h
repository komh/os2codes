/*
 * lresponse() : read a response file without a line length limitation
 *
 * Copyright (C) 2016 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef OS2CODES_LRESPONSE_H
#define OS2CODES_LRESPONSE_H

/**
 * Read a response file without a line length limitation
 * @param argcp pointer to the variable for argument count
 * @param argvp pointer to the variable for argument vector
 */
void lresponse (int *argcp, char ***argvp);

#endif

