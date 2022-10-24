#pragma once

#define	MDS_GF_FDBK		0x169	/* primitive polynomial for GF(256)*/
#define	LFSR1(x) ( ((x) >> 1)  ^ (((x) & 0x01) ?   MDS_GF_FDBK/0x2 : 0))
#define	LFSR2(x) ( ((x) >> 2)  ^ (((x) & 0x02) ?   MDS_GF_FDBK/0x2 : 0)  \
							   ^ (((x) & 0x01) ?   MDS_GF_FDBK/0x4 : 0))

#define	Mx_1(x) ((uint32_t)  (x))		/* force result to dword so << will work */
#define	Mx_X(x) ((uint32_t) ((x) ^            LFSR2(x)))	/* 5B */
#define	Mx_Y(x) ((uint32_t) ((x) ^ LFSR1(x) ^ LFSR2(x)))	/* EF */

#define	M00		Mul_1
#define	M01		Mul_Y
#define	M02		Mul_X
#define	M03		Mul_X

#define	M10		Mul_X
#define	M11		Mul_Y
#define	M12		Mul_Y
#define	M13		Mul_1

#define	M20		Mul_Y
#define	M21		Mul_X
#define	M22		Mul_1
#define	M23		Mul_Y

#define	M30		Mul_Y
#define	M31		Mul_1
#define	M32		Mul_Y
#define	M33		Mul_X

#define	Mul_1	Mx_1
#define	Mul_X	Mx_X
#define	Mul_Y	Mx_Y

#define	RS_GF_FDBK		0x14D

#define	RS_rem(x)		\
	{ uint8_t  b  = (uint8_t) (x >> 24);											 \
	  uint32_t g2 = ((b << 1) ^ ((b & 0x80) ? RS_GF_FDBK : 0 )) & 0xFF;		 \
	  uint32_t g3 = ((b >> 1) & 0x7F) ^ ((b & 1) ? RS_GF_FDBK >> 1 : 0 ) ^ g2 ; \
	  x = (x << 8) ^ (g3 << 24) ^ (g2 << 16) ^ (g3 << 8) ^ b;				 \
	}

#define STORE32L(x, y)  { uint32_t __t = (x); memcpy(y, &__t, 4); }
#define LOAD32L(x, y)   { memcpy(&(x), y, 4); x &= 0xFFFFFFFF; }

#define ROL(x, y) ( (((uint32_t)(x)<<(uint32_t)((y)&31)) | (((uint32_t)(x)&0xFFFFFFFFUL)>>(uint32_t)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROR(x, y) ( ((((uint32_t)(x)&0xFFFFFFFFUL)>>(uint32_t)((y)&31)) | ((uint32_t)(x)<<(uint32_t)(32-((y)&31)))) & 0xFFFFFFFFUL)

#define ROL4L(x) (((x << 1) & 0x0F | ((x & 0x08) >> 3)) | (x & 0xF0))
#define ROL4H(x) (((x << 1) & 0xF0 | ((x & 0x80) >> 3)) | (x & 0x0F))

#define ROR4L(x) (((x >> 1) & 0x0F | ((x & 0x01) << 3)) | (x & 0xF0))
#define ROR4H(x) (((x >> 1) & 0xF0 | ((x & 0x10) << 3)) | (x & 0x0F))