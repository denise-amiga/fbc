/*
 *  libfb - FreeBASIC's runtime library
 *	Copyright (C) 2004-2005 Andre V. T. Vicentini (av1ctor@yahoo.com.br) and others.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * str_tempdescz.c -- temp string descriptor allocation for zstring's
 *
 * chng: may/2005 written [v1ctor]
 *
 */

#include <malloc.h>
#include <string.h>
#include "fb.h"

/*:::::*/
FBCALL FBSTRING *fb_StrAllocTempDescZEx( const char *str, int len )
{
	FBSTRING *dsc;

	FB_STRLOCK();

 	/* alloc a temporary descriptor */
 	dsc = fb_hStrAllocTmpDesc( );

    FB_STRUNLOCK();

    if( dsc == NULL )
    	return &fb_strNullDesc;

	dsc->data = (char *)str;
	dsc->len = len;
	dsc->size = len;

	return dsc;
}

/*:::::*/
FBCALL FBSTRING *fb_StrAllocTempDescZ( const char *str )
{
	int len;

	/* find the true size */
	if( str != NULL )
		len = strlen( str );
	else
		len = 0;

	return fb_StrAllocTempDescZEx( str, len );
}
