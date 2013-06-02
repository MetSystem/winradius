/*
 * rlm_eap_aka.c    Handles that are called from eap for AKA
 *
 * The development of the EAP/AKA support derived/ported/modified from the
 * FreeRadius EAP/SIM support which was funded by Internet Foundation
 * Austria (http://www.nic.at/ipa).
 *
 * Version:     $Id: rlm_eap_aka.c,v 1.0 2004/03/19 02:20:35 mcr Exp $
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
 * Copyright 2003  The FreeRADIUS server project
 *
 */
#include "stdafx.h"
#include <freeradius-devel/ident.h>
RCSID("$Id$")

#include <freeradius-devel/autoconf.h>

#include <stdio.h>
#include <stdlib.h>

#include "eap.h"
#include "eap_types.h"
#include "eap_sim.h"
#include "eap_aka.h"

#include <freeradius-devel/rad_assert.h>
#include <freeradius-devel/3gppsecurity.h>

struct eap_aka_server_state {
	enum eapaka_serverstates state;
	struct eapaka_keys keys;
	int  aka_id;
};


typedef struct rlm_eap_aka_t {
	int r1;
	int r2;
	int r3;
	int r4;
	int r5;
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
} rlm_eap_aka_t;

static const CONF_PARSER module_config[] = {
	{"r1", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, r1), NULL, "32"},
	{"r2", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, r2), NULL, "19"},
	{"r3", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, r3), NULL, "47"},
	{"r4", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, r4), NULL, "73"},
	{"r5", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, r5), NULL, "91"},
	{"c1", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, c1), NULL, "0"},
	{"c2", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, c2), NULL, "1"},
	{"c3", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, c3), NULL, "2"},
	{"c4", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, c4), NULL, "4"},
	{"c5", PW_TYPE_INTEGER, offsetof(rlm_eap_aka_t, c5), NULL, "8"},
	{NULL, -1, 0, NULL, NULL}
};

static rlm_eap_aka_t aka_config;

/*
 *	Detach the module.
 */
static int eap_aka_detach(void *arg)
{
	rlm_eap_aka_t *inst = (rlm_eap_aka_t *) arg;


	free(inst);

	return 0;
}

/*
 *	Attach the module.
 */
static int eap_aka_attach(CONF_SECTION *cs, void **instance)
{


	/*
	 *	Parse the configuration attributes.
	 */
	if (cf_section_parse(cs, &aka_config, module_config) < 0) {
		eap_aka_detach(&aka_config);
		return -1;
	}
	*instance = &aka_config;
	return 0;
}

/*
 * Add value pair to reply
 */
static void add_reply(VALUE_PAIR** vp,
		      const char* name, const char* value, int len)
{
	VALUE_PAIR *reply_attr;
	reply_attr = pairmake(name, "", T_OP_EQ);
	if (!reply_attr) {
#ifdef __HYUN__ORG__
		DEBUG("rlm_eap_aka: "
		      "add_reply failed to create attribute %s: %s\n",
		      name, librad_errstr);
#endif
		return;
	}

	memcpy(reply_attr->vp_strvalue, value, len);
	reply_attr->length = len;
	pairadd(vp, reply_attr);
}

static void eap_aka_state_free(void *opaque)
{
	struct eap_aka_server_state *ess = (struct eap_aka_server_state *)opaque;

	if (!ess) return;

	free(ess);
}

/*
 *	build a reply to be sent.
 */
static int eap_aka_compose(EAP_HANDLER *handler)
{
	/* we will set the ID on requests, since we have to HMAC it */
	handler->eap_ds->set_request_id = 1;

	return map_eapaka_basictypes(handler->request->reply,
				     handler->eap_ds->request);
}

static int eap_aka_getchalans(VALUE_PAIR *vps,struct eap_aka_server_state *ess, REQUEST *request)
{
	VALUE_PAIR *vp;
	u8 k[16],sqn[6],amf[2],op[16],opc[16];

	//read amf from reply attributes
	vp = pairfind(vps, ATTRIBUTE_EAP_AKA_AMF);
	if(vp == NULL)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] Can not find EAP-AKA-AMF", __FILE__, __LINE__);
		return 0;
	}
	if(vp->length != 2)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] EAP-AKA-AMF is not 2-bytes: %d", __FILE__, __LINE__, vp->length);
		return 0;
	}
	memcpy(amf, vp->vp_strvalue, 2);

	//read op from reply attributes
	vp = pairfind(vps, ATTRIBUTE_EAP_AKA_OP);
	if(vp == NULL)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] Can not find EAP-AKA-OP", __FILE__, __LINE__);
		return 0;
	}
	if(vp->length != 16)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] EAP-AKA-OP is not 16-bytes: %d", __FILE__, __LINE__, vp->length);
		return 0;
	}
	memcpy(op, vp->vp_strvalue, 16);
	//read opc from reply attributes
	vp = pairfind(vps, ATTRIBUTE_EAP_AKA_OPC);
	if(vp == NULL || vp->length != 16)
	{
		radlog_request(L_INFO, 0, request, "%s %d - [RLM_EAP_AKA] Can not find EAP-AKA-OPC or EAP-AKA-OPC is not 16-bytes", __FILE__, __LINE__);
		memset(opc, 0, 16);
	}
	else
		memcpy(opc, vp->vp_strvalue, 16);
	//read k from reply attributes
	vp = pairfind(vps, ATTRIBUTE_EAP_AKA_K);
	if(vp == NULL)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] Can not find EAP-AKA-K", __FILE__, __LINE__);
		return 0;
	}
	if(vp->length != 16)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] EAP-AKA-K is not 16-bytes: %d", __FILE__, __LINE__, vp->length);
		return 0;
	}
	memcpy(k, vp->vp_strvalue, 16);
	//read sqn from reply attributes
	vp = pairfind(vps, ATTRIBUTE_EAP_AKA_SQN);
	if(vp == NULL)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] Can not find EAP-AKA-SQN", __FILE__, __LINE__);
		return 0;
	}
	if(vp->length != 6)
	{
		radlog_request(L_ERR, 0, request, "%s %d - [RLM_EAP_AKA] EAP-AKA-SQN is not 6-bytes: %d", __FILE__, __LINE__, vp->length);
		return 0;
	}
	memcpy(sqn, vp->vp_strvalue, 6);

	long long isqn = sqn[5]*pow(2,0) + sqn[4]*pow(2,8) + sqn[3]*pow(2,16) + sqn[2]*pow(2,24) + sqn[1]*pow(2,32) + sqn[0]*pow(2,40);
        isqn = isqn + 32;
        if(isqn > 0xffffffffffff)
                isqn = 0;
        unsigned char newsqn[6];
        newsqn[0] = isqn>>40 & 0xff;
        newsqn[1] = isqn>>32 & 0xff;
        newsqn[2] = isqn>>24 & 0xff;
        newsqn[3] = isqn>>16 & 0xff;
        newsqn[4] = isqn>>8 & 0xff;
        newsqn[5] = isqn & 0xff;	
	memcpy(vp->vp_strvalue, newsqn, 6);
	

	//calc aka ck/ik/ak/res by f2345 
	u8 randnum[16];
    f0(randnum);
    memcpy(ess->keys.rand, randnum, EAPSIM_RAND_SIZE);
	u8 res[8];
    u8 ck[16];
    u8 ik[16];
    u8 ak[6];
	u8 autn[16];
	u8 mac_a[8];
	u8 mac_s[8];
	f1(k, randnum, newsqn, amf, mac_a, op, opc, aka_config.r1, aka_config.c1);
    f2345(k, randnum, res, ck, ik, ak, op, opc, aka_config.r2, aka_config.r3, 
		aka_config.r4, aka_config.c2, aka_config.c3, aka_config.c4);
	CalcAutn(newsqn, ak, amf, mac_a, autn);
    memcpy(ess->keys.ik, ik, 16);
    memcpy(ess->keys.ck, ck, 16);
    memcpy(ess->keys.res, res, 8);
	memcpy(ess->keys.autn, autn, 16);

	//f1star(k, randnum, newsqn, amf, mac_s, op, testmodel[0]);
	f5star(k, randnum, ak, op, opc, aka_config.r5, aka_config.c5);
	//init auts=ak||mac_s, in resp packet auts=(sqnms^ak)||mac_s
	memcpy(ess->keys.auts, ak, 6);
	memcpy(&ess->keys.auts[6], mac_s, 8);
	
	return 1;
}

/*
 * this code sends the challenge itself.
 *
 * Challenges will come from one of three places eventually:
 *
 * 1  from attributes like ATTRIBUTE_EAP_AKA_RANDx
 *            (these might be retrived from a database)
 *
 * 2  from internally implemented AKA authenticators
 *            (a akaple one based upon XOR will be provided)
 *
 * 3  from some kind of SS7 interface.
 *
 * For now, they only come from attributes.
 * It might be that the best way to do 2/3 will be with a different
 * module to generate/calculate things.
 *
 */
static int eap_aka_sendchallenge(EAP_HANDLER *handler)
{
	struct eap_aka_server_state *ess;
	VALUE_PAIR **vps, *newvp;

	ess = (struct eap_aka_server_state *)handler->opaque;
	rad_assert(handler->request != NULL);
	rad_assert(handler->request->reply);

	/* these are the outgoing attributes */
	vps = &handler->request->reply->vps;

	rad_assert(vps != NULL);

  pairdelete(vps,ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_RES);
  pairdelete(vps,ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_RAND);
  pairdelete(vps,ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_AUTN);
  
	/* okay, we got the challenges! Put them into an attribute */
	newvp = paircreate(ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_RAND,
			   PW_TYPE_OCTETS);
	memset(newvp->vp_strvalue,    0, 2); /* clear reserved bytes */
	memcpy(newvp->vp_strvalue+2, ess->keys.rand, EAPSIM_RAND_SIZE);
	newvp->length = 2+EAPSIM_RAND_SIZE;
	pairadd(vps, newvp);

	newvp = paircreate(ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_AUTN,
			   PW_TYPE_OCTETS);
	memset(newvp->vp_strvalue,    0, 2); /* clear reserved bytes */
	memcpy(newvp->vp_strvalue+2, ess->keys.autn, EAPSIM_AUTN_SIZE);
	newvp->length = 2+EAPSIM_AUTN_SIZE;
	pairadd(vps, newvp);

	/* set the EAP_ID - new value */
	newvp = paircreate(ATTRIBUTE_EAP_ID, PW_TYPE_INTEGER);
	newvp->lvalue = ess->aka_id++;
	pairadd(vps, newvp);

	/* make a copy of the identity */
	ess->keys.identitylen = strlen(handler->identity);
	memcpy(ess->keys.identity, handler->identity, ess->keys.identitylen);

	/* all set, calculate keys! */
	eapaka_calculate_keys(&ess->keys);

	/*
	 * need to include an AT_MAC attribute so that it will get
	 * calculated. The NONCE_MT and the MAC are both 16 bytes, so
	 * we store the NONCE_MT in the MAC for the encoder, which
	 * will pull it out before it does the operation.
	 */

	newvp = paircreate(ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_MAC,
			   PW_TYPE_OCTETS);
	memcpy(newvp->vp_strvalue, ess->keys.autn, 16);
	newvp->length = 16;
	pairadd(vps, newvp);

	newvp = paircreate(ATTRIBUTE_EAP_SIM_KEY, PW_TYPE_OCTETS);
	memcpy(newvp->vp_strvalue, ess->keys.K_aut, 16);
	newvp->length = 16;
	pairadd(vps, newvp);

	/* the SUBTYPE, set to challenge. */
	newvp = paircreate(ATTRIBUTE_EAP_SIM_SUBTYPE, PW_TYPE_INTEGER);
	newvp->lvalue = eapaka_challenge;
	pairadd(vps, newvp);


	return 1;
}

#ifndef EAPTLS_MPPE_KEY_LEN
#define EAPTLS_MPPE_KEY_LEN     32
#endif

/*
 * this code sends the success message.
 *
 * the only work to be done is the add the appropriate SEND/RECV
 * radius attributes derived from the MSK.
 *
 */
static int eap_aka_sendsuccess(EAP_HANDLER *handler)
{
        unsigned char *p;
	struct eap_aka_server_state *ess;
	VALUE_PAIR **outvps;
	VALUE_PAIR *newvp;

	/* outvps is the data to the client. */
	outvps= &handler->request->reply->vps;
	ess = (struct eap_aka_server_state *)handler->opaque;

	/* set the EAP_ID - new value */
	newvp = paircreate(ATTRIBUTE_EAP_ID, PW_TYPE_INTEGER);
	newvp->lvalue = ess->aka_id++;
	pairreplace(outvps, newvp);

	p = ess->keys.msk;
	add_reply(outvps, "MS-MPPE-Recv-Key", p, EAPTLS_MPPE_KEY_LEN);
	p += EAPTLS_MPPE_KEY_LEN;
	add_reply(outvps, "MS-MPPE-Send-Key", p, EAPTLS_MPPE_KEY_LEN);
	return 1;
}


/*
 * run the server state machine.
 */
static void eap_aka_stateenter(EAP_HANDLER *handler,
			       struct eap_aka_server_state *ess,
			       enum eapaka_serverstates newstate)
{
	switch(newstate) {
	case eapaka_server_challenge:
		/*
		 * send the EAP-AKA Challenge message.
		 */
		eap_aka_sendchallenge(handler);
		break;

	case eapaka_server_success:
		/*
		 * send the EAP Success message
		 */
  	        eap_aka_sendsuccess(handler);
		handler->eap_ds->request->code = PW_EAP_SUCCESS;
		break;

	default:
		/*
		 * nothing to do for this transition.
		 */
		break;
	}

	ess->state = newstate;

	/* build the target packet */
	eap_aka_compose(handler);
}

/*
 *	Initiate the EAP-AKA session by starting the state machine
 *      and initiating the state.
 */
static int eap_aka_initiate(void *type_data, EAP_HANDLER *handler)
{
	struct eap_aka_server_state *ess;
	VALUE_PAIR *outvps;
	time_t n;

	outvps = handler->request->reply->vps;


	//type_data = type_data;  /* shut up compiler */
	//rlm_eap_aka_t *inst = (rlm_eap_aka_t *) type_data;
	ess = malloc(sizeof(struct eap_aka_server_state));
	if(ess == NULL) {
		radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] no space for eap aka state", __FILE__, __LINE__);
		 pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"No space for eap aka state", T_OP_EQ));
		return 0;
	}

	handler->opaque = ((void *)ess);
	handler->free_opaque = eap_aka_state_free;

	handler->stage = AUTHENTICATE;

	/*
	 * save the keying material, because it could change on a subsequent
	 * retrival.
	 *
	 */
	if(!eap_aka_getchalans(outvps,ess, handler->request))
	{
      radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] can not initiate aka, missing attributes", __FILE__, __LINE__);
	  pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Missing eap-aka attributes", T_OP_EQ));
      return 0;
	}
  
	/*
	 * this value doesn't have be strong, but it is good if it
	 * is different now and then
	 */
	time(&n);
	ess->aka_id = (n & 0xff);

	eap_aka_stateenter(handler, ess, eapaka_server_challenge);

	return 1;
}



/*
 * process an EAP-Aka/Response/Challenge
 *
 * verify that MAC that we received matches what we would have
 * calculated from the packet.
 *
 */
static int process_eap_aka_challenge(EAP_HANDLER *handler, VALUE_PAIR *vps)
{
	struct eap_aka_server_state *ess;
	unsigned char calcmac[EAPSIM_CALCMAC_SIZE];
	VALUE_PAIR *res;

	ess = (struct eap_aka_server_state *)handler->opaque;

	/* verify the MAC, now that we have all the keys. */
	if(eapsim_checkmac(vps, ess->keys.K_aut,NULL,0,calcmac)) {
		DEBUG2("MAC check succeed\n");
	} else {
		int i, j;
		unsigned char macline[20*3];
		char *m = macline;

		j=0;
		for (i = 0; i < EAPSIM_CALCMAC_SIZE; i++) {
			if(j==4) {
			  *m++ = '_';
			  j=0;
			}
			j++;

			sprintf(m, "%02x", calcmac[i]);
			m = m + strlen(m);
		}
		radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] calculated MAC (%s) did not match", __FILE__, __LINE__, macline);
		pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Calculated MAC not match", T_OP_EQ));
		return 0;
	}
	res = pairfind(vps, ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_RES);
	if((res == NULL) || (res->length > 18 || res->length < 6))
        {
      /* bad, we can't find stuff! */
		radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] AT_RES missing or invalid length from eap aka challenge response", __FILE__, __LINE__);
	  	pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Invalid AT_RES from eap-aka challenge response", T_OP_EQ));
		return 0;
	}
#if 0	
  if(memcmp(&res->vp_strvalue[2], ess->keys.res, 16))
  {
		int i, j;
		unsigned char resline[16*3];
		char *m = resline;

		j=0;
		for (i = 0; i < EAPSIM_RES_SIZE; i++)
    {
        if(j==4) {
            *m++ = '_';
            j=0;
        }
        j++;
        
        sprintf(m, "%02x", res->vp_strvalue[2+i]);
        m = m + strlen(m);
    }
    DEBUG2("AT_RES supplied (%s) does match configured value",resline);
    return(1);
	}
#else	
  //if(memcmp(&res->vp_strvalue[2], ess->keys.res, res->length - 2))

  if(memcmp(&res->vp_strvalue[2], ess->keys.res, 8))
  {
		int i, j;
		unsigned char resline[16*3];
		char *m = resline;

		j=0;
		for (i = 0; i < res->length; i++)
    {
        if(j==4) {
            *m++ = '_';
            j=0;
        }
        j++;
        
        sprintf(m, "%02x", res->vp_strvalue[2+i]);
        m = m + strlen(m);
    }
    radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] AT_RES supplied (%s) does match configured value, res_len:( %d)",__FILE__, __LINE__, resline,res->length);
	pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"AT_RES not match", T_OP_EQ));
    return(1);
	}
#endif	
  else
  {
      DEBUG2("AT_RES supplied matches configured value");
  }


	/* everything looks good, change states */
	eap_aka_stateenter(handler, ess, eapaka_server_success);
	return 1;
}


static int process_eap_aka_sqn_sync(EAP_HANDLER *handler, VALUE_PAIR *vps)
{
	struct eap_aka_server_state *ess;
	//unsigned char calcmac[EAPSIM_CALCMAC_SIZE];
	VALUE_PAIR *auts;

	ess = (struct eap_aka_server_state *)handler->opaque;
	//find AUTS
	auts = pairfind(vps, ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_AUTS);
	if((auts == NULL) || (auts->length != 14))
    {
      /* bad, we can't find stuff! */
		radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] AT_AUTS missing or invalid length from eap aka sync failure", __FILE__, __LINE__);
	  	pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Invalid AT_AUTS from eap-aka challenge response", T_OP_EQ));
		return 0;
	}
	//not check mac_s auts=(sqn^ak)||mac_s
    /*if(memcmp(&auts->vp_strvalue[6], &ess->keys.auts[6], 8))
    {
  		DEBUG2("AT_AUTS supplied (%s) does match configured value");
    	return(1);
	 }*/

  	  u8 sqnms[6];
	  u8 resyncak[6];
	  memcpy(resyncak, ess->keys.auts, 6);
	  memcpy(sqnms, auts->vp_strvalue, 6);
	  //calc sqnms
	  memxor(sqnms, resyncak, 6);
	  //recalc attributes
      VALUE_PAIR *outvps = handler->request->reply->vps;
	  VALUE_PAIR *vp = pairfind(outvps, ATTRIBUTE_EAP_AKA_SQN);
	  if(vp == NULL)
	  {
		  radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] Can not find EAP-AKA-SQN", __FILE__, __LINE__);
		  pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Can not find SQN from reply packet", T_OP_EQ));
		  return 0;
	  }
	  if(vp->length != 6)
	  {
		  radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] EAP-AKA-SQN is not 6-bytes: %d", __FILE__, __LINE__, vp->length);
		  pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Invalid SQN from reply packet", T_OP_EQ));
		  return 0;
	  }
	  memcpy(vp->vp_strvalue, sqnms, 6);
	  eap_aka_getchalans(outvps, ess, handler->request);

	/* everything looks good, change states */
	eap_aka_stateenter(handler, ess, eapaka_server_challenge);
	return 1;
}


/*
 *	Authenticate a previously sent challenge.
 */
static int eap_aka_authenticate(void *arg, EAP_HANDLER *handler)
{
	struct eap_aka_server_state *ess;
	VALUE_PAIR *vp, *vps;
	enum eapaka_subtype subtype;
	int success;

	arg = arg; /* shut up compiler */

	ess = (struct eap_aka_server_state *)handler->opaque;

	/* vps is the data from the client */
	vps = handler->request->packet->vps;

	success= unmap_eapaka_basictypes(handler->request->packet,
					 handler->eap_ds->response->type.data,
					 handler->eap_ds->response->type.length);

	if(!success) {
	  return 0;
	}

	/* see what kind of message we have gotten */
	if((vp = pairfind(vps, ATTRIBUTE_EAP_SIM_SUBTYPE)) == NULL)
	{
		radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] no subtype attribute was created, message dropped", __FILE__, __LINE__);
		pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"No subtype attribute from request", T_OP_EQ));
		return 0;
	}
	subtype = vp->lvalue;

	switch(ess->state) {
	case eapaka_server_challenge:
		switch(subtype) {
		default:
			/*
			 * pretty much anything else here is illegal,
			 * so we will retransmit the request.
			 */
			eap_aka_stateenter(handler, ess, eapaka_server_challenge);
			return 1;

    case eapaka_authentication_reject:
        /* The client did not like the AUTN the server sent so give up.*/
        radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] Peer Rejected AT_AUTN", __FILE__, __LINE__);
		pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Peer rejected AT_AUTN", T_OP_EQ));
        return 0;
    case eapaka_client_error:
        if((vp = pairfind(vps, ATTRIBUTE_EAP_SIM_BASE+PW_EAP_SIM_CLIENT_ERROR_CODE)) == NULL)
        {
            radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] Peer rejected AKA-Challenge with client-error message but "
                   "has not supplied a client error code", __FILE__, __LINE__);
        }
        else
        {
            char client_error_code_string[64];
            radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] Client rejected AKA-Challenge with client-error message client-error-code = %s", __FILE__, __LINE__, 
                   aka_clienterrorcode2name(vp->lvalue,
                                            client_error_code_string,
                                            64));
        }
		pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Peer rejected AKA-Challenge with client-error-code", T_OP_EQ));
        return 0;
    case eapaka_challenge:
			/*
			 * a response to our EAP-Aka/Request/Challenge!
			 *
			 */
			return process_eap_aka_challenge(handler, vps);

	case eapaka_synchronization_failure:
			return process_eap_aka_sqn_sync(handler, vps);
		}
		break;

	default:
		/* if we get into some other state, die, as this
		 * is a coding error!
		 */
		radlog_request(L_ERR, 0, handler->request, "%s %d - [RLM_EAP_AKA] illegal-unknown state reached in eap_aka_authenticate\n", __FILE__, __LINE__);
		pairadd(&handler->request->packet->vps, pairmake("Kineto-Access-Detail", 
				"Invalid state from request", T_OP_EQ));
		abort();
 	}

	return 0;
}

/*
 *	The module name should be the only globally exported symbol.
 *	That is, everything else should be 'static'.
 */
EAP_TYPE rlm_eap_aka = {
	"eap_aka",
	eap_aka_attach,				
	eap_aka_initiate,		/* Start the initial request */
	NULL,				/* XXX authorization */
	eap_aka_authenticate,		/* authentication */
	eap_aka_detach				
};

/*
 * $Log: rlm_eap_aka.c,v $
 *
 * Local Variables:
 * c-file-style: "linux"
 * End Variables:
 *
 */
