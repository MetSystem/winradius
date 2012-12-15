/*
 * mschap.c
 *
 * Version:	$Id$
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 * Copyright 2000,2001,2006,2010  The FreeRADIUS server project
 */


/*
 *  This implements MS-CHAP, as described in RFC 2548
 *
 *  http://www.freeradius.org/rfc/rfc2548.txt
 *
 */
#include "stdafx.h"
#include	<freeradius-devel/ident.h>
RCSID("$Id$")

#include	<freeradius-devel/radiusd.h>
#include	<freeradius-devel/modules.h>
#include	<freeradius-devel/rad_assert.h>
#include        <freeradius-devel/md5.h>
#include        <freeradius-devel/sha1.h>

#include 	<ctype.h>

#include	"smbdes.h"



/*
 *	challenge_hash() is used by mschap2() and auth_response()
 *	implements RFC2759 ChallengeHash()
 *	generates 64 bit challenge
 */
void mschap_challenge_hash( const uint8_t *peer_challenge,
			    const uint8_t *auth_challenge,
			    const char *user_name, uint8_t *challenge )
{
	fr_SHA1_CTX Context;
	uint8_t hash[20];

	fr_SHA1Init(&Context);
	fr_SHA1Update(&Context, peer_challenge, 16);
	fr_SHA1Update(&Context, auth_challenge, 16);
	fr_SHA1Update(&Context, (const uint8_t *) user_name,
		      strlen(user_name));
	fr_SHA1Final(hash, &Context);
	memcpy(challenge, hash, 8);
}

