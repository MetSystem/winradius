#ifndef _3GPPSECURITY_H
#define _3GPPSECURITY_H

/*-------------------------------------------------------------------
 *          algorithms f1, f1*, f2, f3, f4, f5, f5*
 *-------------------------------------------------------------------
 *
 *  3GPP authentication and key agreement functions f1, f1*, f2, f3, f4, f5 and f5*.  
 *  This is a byte-oriented implementation of the functions, and of the block
 *  cipher kernel function Rijndael.
 *
 *  This has been coded for clarity, not necessarily for efficiency.
 *
 *  The functions f2, f3, f4 and f5 share the same inputs and have 
 *  been coded together as a single function.  f1, f1* and f5* are
 *  all coded separately.
 *
 *-----------------------------------------------------------------*/

typedef unsigned char u8;

/*--------------------------- prototypes --------------------------*/
void f0(u8 rand[16]);

void f1(u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2],
			u8 mac_a[8], u8 op[16], u8 opc[16], int r1, int c1);
void f2345(u8 k[16], u8 rand[16], u8 res[8], u8 ck[16], u8 ik[16], 
				u8 ak[6], u8 op[16], u8 opc[16], int r2, int r3, 
				int r4, int c2, int c3, int c4);
void f1star(u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], 
				u8 mac_s[8], u8 op[16], u8 opc[16], int r1, int c1);
void f5star(u8 k[16], u8 rand[16], u8 ak[6], u8 op[16], 
				u8 opc[16], int r5, int c5);
void ComputeOPc(u8 op[16], u8 op_c[16], u8 opc[16]);
void RijndaelKeySchedule(u8 key[16]);
void RijndaelEncrypt(u8 input[16], u8 output[16]);
void CalcAutn(u8 sqn[6], u8 ak[6], u8 amf[2], u8 mac[8], u8 autn[16]);
void memxor(u8 dest[],u8 src[], unsigned int n);

#endif /* _3GPPSECURITY_H */

