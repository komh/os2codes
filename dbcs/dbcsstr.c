/*
 * DBCS string functions
 *
 * Copyright (C) 2015 KO Myung-Hun <komh@chollian.net>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#define INCL_DOSNLS
#include <os2.h>

#include <string.h>

static int m_fIsEnabled = 0;

/* Array of a character length, mapping a character code to a length */
static char m_achCharLen[ 256 ];

/* Note unsigned char is needed to access m_achCharLen. */
#define GET_CHAR_LEN( ch ) ( m_achCharLen[( unsigned char )( ch )])

__attribute__((constructor))    /* Call at startup. */
static void initCharLen( void )
{
    COUNTRYCODE cc = { 0, }; /* Default system language and current code page */
    UCHAR       auchDBCSLeadBytes[ 12 ]; /* A vector for lead-bytes range */
    int         i, j;

    /* Assume SBCS by default */
    memset( m_achCharLen, 1, sizeof( m_achCharLen ));
    memset( auchDBCSLeadBytes, 0, sizeof( auchDBCSLeadBytes ));

    DosQueryDBCSEnv( sizeof( auchDBCSLeadBytes ), &cc, auchDBCSLeadBytes );

    /* Determine if DBCS is enabled. */
    m_fIsEnabled = auchDBCSLeadBytes[ 0 ] || auchDBCSLeadBytes[ 1 ];

    /* Lead-bytes ranges end with two zero byte */
    for( i = 0; auchDBCSLeadBytes[ i ] || auchDBCSLeadBytes[ i + 1 ]; i += 2 )
    {
        /* If a DBCS character, set its character size to 2. */
        for( j = auchDBCSLeadBytes[ i ]; j <= auchDBCSLeadBytes[ i + 1 ]; j++ )
            m_achCharLen[ j ] = 2;
    }
}

/**
 * Test if DBCS is enabled
 *
 * @return non-zero on enabled, otherwise 0 on disabled
 */
int dbcsIsEnabled( void )
{
    return m_fIsEnabled;
}

/**
 * Calculate a length of a string containing DBCS characters
 *
 * @param[in] str a string containing DBCS characters
 *
 * @return a length of a string pointed by @a str
 */
int dbcsStrlen( const char *str )
{
    const char *p;
    int len;

    for( len = 0, p = str; *p;)
    {
        p += GET_CHAR_LEN( *p );    /* DBCS lead-byte ? */
        len++;

        if( !p[ -1 ])   /* For the case of a broken DBCS character,
                           check not to pass an end of a string. */
            break;
    }

    return len;
}

/**
 * Check if a chracter at @a pos of @a str is a DBCS lead byte
 *
 * @param[in] str a string containing DBCS characters
 * @param[in] pos a index of @a str
 *
 * @return 1 if DBCS lead byte, or 0 if SBCS
 */
int dbcsIsLeadByte( const char *str, int pos )
{
    const char *end = str + pos;

    /* Walk through from first. */
    while( str < end )
        str += GET_CHAR_LEN( *str );

    /* Check a character at position. */
    return str == end && GET_CHAR_LEN( *str ) == 2;
}

/**
 * Check if a character at @a pos of @a str is a DBCS lead byte
 *
 * @param[in] str a string containing DBCS characters
 * @param[in] pos a index of @a str
 *
 * @return 1 if DBCS lead byte, otherwise 0
 */
int dbcsIsTrailByte( const char *str, int pos )
{
    const char *end = str + pos;

    /* Walk through from first. */
    while( str < end )
        str += GET_CHAR_LEN( *str );

    /* Passed a position ? Then it is a trail-byte. For example, if str is
       LTLT, and pos is 1, then str is at index 2 and end is at index 1. */
    return str > end;
}

