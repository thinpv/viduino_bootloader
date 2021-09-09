#ifndef MD5SUM_H__
#define MD5SUM_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned int uint;
typedef unsigned char byte;

typedef unsigned long ulong;
typedef unsigned char uchar;

typedef struct Table
{
	uint	sin;	/* integer part of 4294967296 times abs(sin(i)) */
	byte	x;	/* index into data block */
	byte	rot;	/* amount to rotate left by */
}Table;

typedef struct MD5state
{
	uint len;
	uint state[4];
	uint done;
}MD5state;

/*
 *  I require len to be a multiple of 64 for all but
 *  the last call
 */
MD5state* md5(byte *p, uint len, byte *digest, MD5state *s);

/*
 *	encodes input (uint) into output (byte). Assumes len is
 *	a multiple of 4.
 */
void encode(byte *output, uint *input, uint len);

/*
 *	decodes input (byte) into output (uint). Assumes len is
 *	a multiple of 4.
 */
void decode(uint *output, byte *input, uint len);

int dec64(uchar *out, char *in, int n);

int enc64(char *out, uchar *in, int n);

void md5sum_init(MD5state *s);

#endif
