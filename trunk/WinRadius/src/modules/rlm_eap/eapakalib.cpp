/*
 * eapakalib.c    based upon draft-haverinen-pppext-eap-aka-11.txt.
 *
 * The development of the EAP/AKA support was funded by Internet Foundation
 * Austria (http://www.nic.at/ipa).
 *
 * code common to EAP-AKA clients and to servers.
 *
 * Version:     $Id: eapakalib.c,v 1.7.2.2 2005/01/26 20:31:31 aland Exp $
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
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Copyright 2000-2003  The FreeRADIUS server project
 * Copyright 2003  Michael Richardson <mcr@sandelman.ottawa.on.ca>
 */

/*
 *  EAP-AKA PACKET FORMAT
 *  ------- ------ ------
 *
 * EAP Request and Response Packet Format
 * --- ------- --- -------- ------ ------
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Code      |  Identifier   |            Length             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Type      |  AKA-Type     |   AKA-Length  |     value ... |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * with AKA-Type/AKA-Length/Value... repeating. AKA-Length is in units
 * of 32 bits, and includes the Aka-Type/Aka-Length fields.
 *
 * The AKA-Type's are mapped to ATTRIBUTE_EAP_SIM_BASE+Aka-type and
 * unmapped by these functions.
 *
 */

#include "stdafx.h"
#include <process.h>
#include "eap_types.h"
#include "eap_sim.h"
#include "eap_aka.h"
#include <freeradius-devel/libradius.h>
#include <freeradius-devel/sha1.h>

static const char rcsid[] = "$Id: eapakalib.c,v 1.7.2.2 2005/01/26 20:31:31 aland Exp $";

/*
 * given a radius request with many attribues in the EAP-AKA range, build
 * them all into a single EAP-AKA body.
 *
 */
int map_eapaka_basictypes(RADIUS_PACKET *r, EAP_PACKET *ep)
{
	VALUE_PAIR       *vp;
	int               encoded_size;
	uint8_t          *encodedmsg, *attr;
	unsigned int      id, eapcode;
	unsigned char    *macspace;
	unsigned char     subtype;

	macspace = NULL;

	/*
	 * encodedmsg is now an EAP-AKA message.
	 * it might be too big for putting into an EAP-Type-AKA
	 *
	 */
	vp = pairfind(r->vps, ATTRIBUTE_EAP_SIM_SUBTYPE);
	if(vp == NULL)
	{
		subtype = eapaka_challenge;
	}
	else
	{
		subtype = vp->lvalue;
	}

	vp = pairfind(r->vps, ATTRIBUTE_EAP_ID);
	if(vp == NULL)
	{
		id = ((int)getpid() & 0xff);
	}
	else
  {
		id = vp->lvalue;
	}

	vp = pairfind(r->vps, ATTRIBUTE_EAP_CODE);
	if(vp == NULL)
	{
		eapcode = PW_EAP_REQUEST;
	}
	else
	{
		eapcode = vp->lvalue;
	}


	/*
	 * take a walk through the attribute list to see how much space
	 * that we need to encode all of this.
	 */
	encoded_size = 0;
	for(vp = r->vps; vp != NULL; vp = vp->next)
	{
		int roundedlen;
		int vplen;

		if(vp->attribute < ATTRIBUTE_EAP_SIM_BASE ||
		   vp->attribute >= ATTRIBUTE_EAP_SIM_BASE+256)
		{
			continue;
		}

		vplen = vp->length;

		/*
		 * the AT_MAC attribute is a bit different, when we get to this
		 * attribute, we pull the contents out, save it for later
		 * processing, set the size to 16 bytes (plus 2 bytes padding).
		 *
 		 * At this point, we only care about the size.
		 */
		if(vp->attribute == ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_MAC) {
			vplen = 18;
		}

		/* round up to next multiple of 4, after taking in
		 * account the type and length bytes
		 */
		roundedlen = (vplen + 2 + 3) & ~3;
		encoded_size += roundedlen;
	}

	if (ep->code != PW_EAP_SUCCESS)
		ep->code = eapcode;
	ep->id = (id & 0xff);
	ep->type.type = PW_EAP_AKA;

	/*
	 * if no attributes were found, do very little.
	 *
	 */
	if(encoded_size == 0)
	{
	        encodedmsg = (uint8_t *)malloc(3);
		/* FIX: could be NULL */

		encodedmsg[0]=subtype;
		encodedmsg[1]=0;
		encodedmsg[2]=0;

		ep->type.length = 3;
		ep->type.data = encodedmsg;

		return 0;
	}


	/*
	 * figured out the length, so malloc some space for the results.
	 *
	 * Note that we do not bother going through an "EAP" stage, which
	 * is a bit strange compared to the unmap, which expects to see
	 * an EAP-AKA virtual attributes.
	 *
	 * EAP is 1-code, 1-identifier, 2-length, 1-type = 5 overhead.
	 *
	 * AKA code adds a subtype, and 2 bytes of reserved = 3.
	 *
	 */

	/* malloc space for it */

	encoded_size += 3;
	encodedmsg = (uint8_t *)malloc(encoded_size);
	if (encodedmsg == NULL) {
		radlog(L_ERR, "%s %d - [RLM_EAP] out of memory allocating %d bytes", __FILE__, __LINE__, encoded_size+5);
		return 0;
	}
	memset(encodedmsg, 0, encoded_size);

	/*
	 * now walk the attributes again, sticking them in.
	 *
	 * we go three bytes into the encoded message, because there are two
	 * bytes of reserved, and we will fill the "subtype" in later.
	 *
	 */
	attr = encodedmsg+3;

	for(vp = r->vps; vp != NULL; vp = vp->next)
	{
		int roundedlen;

		if(vp->attribute < ATTRIBUTE_EAP_SIM_BASE ||
		   vp->attribute >= ATTRIBUTE_EAP_SIM_BASE+256)
		{
			continue;
		}

		/*
		 * the AT_MAC attribute is a bit different, when we get to this
		 * attribute, we pull the contents out, set the size to 16 bytes
     * (plus 2 bytes padding).
		 *
 		 * At this point, we put in zeros, and remember where the
		 * sixteen bytes go.
		 */
		if(vp->attribute == ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_MAC) {
			roundedlen = 20;
			memset(&attr[2], 0, 18);
			macspace = &attr[4];
		}
		else {
			roundedlen = (vp->length + 2 + 3) & ~3;
			memset(attr, 0, roundedlen);
			memcpy(&attr[2], vp->vp_strvalue, vp->length);
		}
		attr[0] = vp->attribute - ATTRIBUTE_EAP_SIM_BASE;
		attr[1] = roundedlen >> 2;

		attr += roundedlen;
	}

	encodedmsg[0] = subtype;

	ep->type.length = encoded_size;
	ep->type.data = encodedmsg;

	/*
	 * if macspace was set and we have a key,
	 * then we should calculate the HMAC-SHA1 of the resulting EAP-AKA
	 * packet
	 */
	vp = pairfind(r->vps, ATTRIBUTE_EAP_SIM_KEY);
	if(macspace != NULL && vp != NULL)
	{
		unsigned char   *buffer;
		eap_packet_t	*hdr;
		uint16_t         hmaclen, total_length = 0;
		unsigned char    sha1digest[20];

		total_length = EAP_HEADER_LEN + 1 + encoded_size;
		hmaclen = total_length;
		buffer = (unsigned char *)malloc(hmaclen);
		hdr = (eap_packet_t *)buffer;
		if (!hdr) {
			radlog(L_ERR, "%s %d - [RLM_EAP] out of memory", __FILE__, __LINE__);
			free(encodedmsg);
			return 0;
		}

		hdr->code = eapcode & 0xFF;
		hdr->id = (id & 0xFF);
		total_length = htons(total_length);
		memcpy(hdr->length, &total_length, sizeof(uint16_t));

		hdr->data[0] = PW_EAP_AKA;

		/* copy the data */
		memcpy(&hdr->data[1], encodedmsg, encoded_size);

		/* HMAC it! */
		fr_hmac_sha1(buffer, hmaclen,
			       (uint8_t *)vp->vp_strvalue, vp->length,
			       sha1digest);

		/* done with the buffer, free it */
		free(buffer);

		/* now copy the digest to where it belongs in the AT_MAC */
                /* note that it is truncated to 128-bits */
		memcpy(macspace, sha1digest, 16);
	}

	/* if we had an AT_MAC and no key, then fail */
	if(macspace != NULL && vp == NULL)
	{
		if(encodedmsg != NULL)
			free(encodedmsg);
		return 0;
	}

	return 1;
}

int map_eapaka_types(RADIUS_PACKET *r)
{
	EAP_PACKET ep;
	int ret;

	memset(&ep, 0, sizeof(ep));
	ret = map_eapaka_basictypes(r, &ep);
	if(ret != 1) {
		return ret;
	}
	eap_basic_compose(r, &ep);

	return 1;
}

/*
 * given a radius request with an EAP-AKA body, decode it into TLV pairs
 *
 * return value is TRUE if it succeeded, false if there was something
 * wrong and the packet should be discarded.
 *
 */
int unmap_eapaka_basictypes(RADIUS_PACKET *r,
			    uint8_t *attr, unsigned int attrlen)
{
	VALUE_PAIR              *newvp;
	int                     eapaka_attribute;
	unsigned int            eapaka_len;
	int                     es_attribute_count;

	es_attribute_count=0;

	/* big enough to have even a single attribute */
	if(attrlen < 3) {
		radlog(L_ERR, "%s %d - [RLM_EAP] EAP-Aka attribute too short: %d < 3", __FILE__, __LINE__, attrlen);
		return 0;
	}
  /***********************************************************************/
  /* If the subtype is not authentication reject then it must be at least*/
  /* 5 long.                                                             */
  /***********************************************************************/
  if (attr[0] != eapaka_authentication_reject)
  {
      if(attrlen < 5) {
          radlog(L_ERR, "%s %d - [RLM_EAP] EAP-Aka attribute too short: %d < 5", __FILE__, __LINE__, attrlen);
          return 0;
      }
  }

	newvp = paircreate(ATTRIBUTE_EAP_SIM_SUBTYPE, PW_TYPE_INTEGER);
	newvp->lvalue = attr[0];
	newvp->length = 1;
	pairadd(&(r->vps), newvp);

	attr     += 3;
	attrlen  -= 3;

	/* now, loop processing each attribute that we find */
	while(attrlen > 0)
	{
		if(attrlen < 2) {
			radlog(L_ERR, "%s %d - [RLM_EAP] EAP-Aka attribute %d too short: %d < 2", __FILE__, __LINE__, es_attribute_count, attrlen);
			return 0;
		}

		eapaka_attribute = attr[0];
		eapaka_len = attr[1] * 4;

		if(eapaka_len > attrlen) {
			radlog(L_ERR, "%s %d - [RLM_EAP] EAP-Aka attribute %d (no.%d) has length longer than data (%d > %d)", __FILE__, __LINE__
			       , eapaka_attribute
			       , es_attribute_count, eapaka_len, attrlen);
			return 0;
		}

		if(eapaka_len > MAX_STRING_LEN) {
			eapaka_len = MAX_STRING_LEN;
		}
		if (eapaka_len < 2) {
			radlog(L_ERR, "%s %d - [RLM_EAP] EAP-Aka attribute %d (no.%d) has length too small", __FILE__, __LINE__, 
			       eapaka_attribute, es_attribute_count);
			       return 0;
		}

		newvp = paircreate(eapaka_attribute+ATTRIBUTE_EAP_SIM_BASE, PW_TYPE_OCTETS);
		memcpy(newvp->vp_strvalue, &attr[2], eapaka_len-2);
		newvp->length = eapaka_len-2;
		pairadd(&(r->vps), newvp);
		newvp = NULL;

		/* advance pointers, decrement length */
		attr += eapaka_len;
		attrlen  -= eapaka_len;
		es_attribute_count++;
	}
	return 1;
}

int unmap_eapaka_types(RADIUS_PACKET *r)
{
	VALUE_PAIR             *esvp;

	esvp = pairfind(r->vps, ATTRIBUTE_EAP_BASE+PW_EAP_AKA);
	if (esvp == NULL) {
		radlog(L_ERR, "%s %d - [RLM_EAP] EAP-Aka attribute not found", __FILE__, __LINE__);
		return 0;
	}

	return unmap_eapaka_basictypes(r, (uint8_t *)esvp->vp_strvalue, esvp->length);
}

/*
 * definitions changed to take a buffer for unknowns
 * as this is more thread safe.
 */
const char *akastates[]={ "init", "start", "success",NULL };

const char *aka_state2name(enum eapaka_clientstates state,
			   char *statenamebuf,
			   int   statenamebuflen)
{
	if(state >= eapaka_client_maxstates)
	{
		snprintf(statenamebuf, statenamebuflen,
			 "eapstate:%d", state);
		return statenamebuf;
	}
	else
	{
		return akastates[state];
	}
}
const char *akaclienterrorcodes[]={ "unable to comply", NULL };

const char *aka_clienterrorcode2name(enum eapaka_clienterrorcodes errorcode,
			   char *errorcodenamebuf,
			   int   errorcodenamebuflen)
{
	if(errorcode >= eapaka_client_maxclienterrorcodes)
	{
		snprintf(errorcodenamebuf, errorcodenamebuflen,
			 "eapclienterrorcode:%d", errorcode);
		return errorcodenamebuf;
	}
	else
	{
		return akaclienterrorcodes[errorcode];
	}
}

static const char *subtypes[]={ "subtype0",
                                "AKA-challenge",
                                "AKA-authentication-reject",
                                "subtype3",
                                "subtype4",
                                "subtype5",
                                "subtype6",
                                "subtype7",
                                "subtype8",
                                "subtype9",
                                "start",
                                "challenge",
                                "notification",
                                "reauth",
                                "AKA-client-error",
                                NULL };

const char *aka_subtype2name(enum eapaka_subtype subtype,
			     char *subtypenamebuf,
			     int   subtypenamebuflen)
{
	if(subtype >= eapaka_max_subtype)
	{
		snprintf(subtypenamebuf, subtypenamebuflen,
			 "illegal-subtype:%d", subtype);
		return subtypenamebuf;
	}
	else
	{
		return subtypes[subtype];
	}
}



void eapaka_calculate_keys(struct eapaka_keys *ek)
{
	fr_SHA1_CTX context;
	uint8_t fk[160];
	unsigned char buf[256];
	unsigned char *p;
	unsigned int  blen;

	p = buf;
	memcpy(p, ek->identity, ek->identitylen);   p = p+ek->identitylen;
	memcpy(p, ek->ik, EAPAKA_IK_SIZE);       p = p+EAPAKA_IK_SIZE;
	memcpy(p, ek->ck, EAPAKA_CK_SIZE);       p = p+EAPAKA_CK_SIZE;

	blen = p - buf;

	/* do the master key first */
	fr_SHA1Init(&context);
	fr_SHA1Update(&context, buf, blen);
	fr_SHA1Final(ek->master_key, &context);

	/*
   * now use the PRF to expand it, generated K_aut, K_encr,
	 * MSK and EMSK.
	 */
	fips186_2prf(ek->master_key, fk);

	/* split up the result */
	memcpy(ek->K_encr, fk +  0, 16);    /* 128 bits for encryption    */
	memcpy(ek->K_aut,  fk + 16, EAPSIM_AUTH_SIZE); /*128 bits for auth */
	memcpy(ek->msk,    fk + 32, 64);  /* 64 bytes for Master Session Key */
	memcpy(ek->emsk,   fk + 96, 64);  /* 64- extended Master Session Key */
}
