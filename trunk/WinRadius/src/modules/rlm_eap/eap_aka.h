/*
 * eap_aka.h    Header file containing the EAP-AKA types
 *
 * Version:     $Id: eap_aka.h,v Exp $
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
 * Copyright 2003  Michael Richardson <mcr@sandelman.ottawa.on.ca>
 * Copyright 2006  Brian Phaneuf <bphaneuf@reefpoint.com>
 *
 */
#ifndef _EAP_AKA_H
#define _EAP_AKA_H

#include <freeradius-devel/ident.h>
RCSIDH(eap_aka_h, "$Id$")

#include "eap_types.h"
#include "eap_sim.h"

/* base for dictionary values */

enum eapaka_subtype {
  eapaka_challenge   = 1,
  eapaka_authentication_reject = 2,
  eapaka_synchronization_failure = 4,	//add by wuweijie 2012-07-12 for sqn sync
  eapaka_notification= 12,
  eapaka_reauth      = 13,
  eapaka_client_error = 14,
  eapaka_max_subtype
};

enum eapaka_clientstates {
  eapaka_client_init = 0,
  eapaka_client_start = 1,
  eapaka_client_maxstates
};

enum eapaka_clienterrorcodes {
  eapaka_client_unable_to_comply = 0,
  eapaka_client_maxclienterrorcodes
};

/* server states
 *
 * in server_start, we send a EAP-AKA Start message.
 *
 */
enum eapaka_serverstates {
  eapaka_server_start = 0,
  eapaka_server_challenge=1,
  eapaka_server_success=2,
  eapaka_server_maxstates
};

/*
 * interfaces in eapakalib.c
 */
extern int map_eapaka_types(RADIUS_PACKET *r);
extern int map_eapaka_basictypes(RADIUS_PACKET *r, EAP_PACKET *ep);
extern int unmap_eapaka_types(RADIUS_PACKET *r);
extern const char *aka_state2name(enum eapaka_clientstates state, char *buf, int buflen);
extern const char *aka_clienterrorcode2name(enum eapaka_clienterrorcodes errorcode,
                                            char *errorcodenamebuf,
                                            int   errorcodenamebuflen);
extern const char *aka_subtype2name(enum eapaka_subtype subtype, char *buf, int buflen);
extern int unmap_eapaka_basictypes(RADIUS_PACKET *r,
				   uint8_t *attr, unsigned int attrlen);


/************************/
/*   CRYPTO FUNCTIONS   */
/************************/

/*
 * key derivation functions/structures
 *
 */

#define EAPAKA_IK_SIZE      16
#define EAPAKA_CK_SIZE      16

struct eapaka_keys {
  /* inputs */
  unsigned char identity[MAX_STRING_LEN];
  unsigned int  identitylen;
  unsigned char autn[EAPSIM_AUTN_SIZE];
  unsigned char rand[EAPSIM_RAND_SIZE];
  unsigned char ik[EAPAKA_IK_SIZE];
  unsigned char ck[EAPAKA_CK_SIZE];
  unsigned char res[EAPSIM_RES_SIZE];
  unsigned char auts[14]; //add by wuweijie 2012-07-20 for sqn sync
  /*unsigned char ak[6];
  unsigned char k[16];
  unsigned char sqn[6];
  unsigned char amf[2];
  unsigned char op[16];
  unsigned char mac_a[8];
  */

  /* outputs */
  unsigned char master_key[20];
  unsigned char K_aut[EAPSIM_AUTH_SIZE];
  unsigned char K_encr[16];
  unsigned char msk[64];
  unsigned char emsk[64];
};


/*
 * interfaces in eapakalib.c
 */
extern int  eapaka_checkmac(VALUE_PAIR *rvps,
			    uint8_t key[8],
			    uint8_t calcmac[20]);

/*
 * in eapcrypto.c
 */
extern void eapaka_calculate_keys(struct eapaka_keys *ek);
extern void eapaka_dump_mk(struct eapaka_keys *ek);


#endif /* _EAP_AKA_H */
