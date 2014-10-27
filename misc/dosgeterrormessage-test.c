/*
 * DosGetErrorMessage() test program
 *
 * Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include <stdio.h>

#include "dosgeterrormessage.h"

#define ERROR_CODE  3175

int main( void )
{
    char buf[ 512 ];

    printf("Error message for error code %d :\n%s\n",
           ERROR_CODE, DosGetErrorMessage( ERROR_CODE, buf, sizeof( buf )));

    return 0;
}
