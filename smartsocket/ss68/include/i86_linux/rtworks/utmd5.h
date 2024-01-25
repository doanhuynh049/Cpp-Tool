/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/utmd5.h#1 $
 */

/* $ $ */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */
 
#ifndef _RTWORKS_MD5_h
#define _RTWORKS_MD5_h

#include <rtworks/config.h>


/* MD5 context. */
typedef struct {
  T_UINT4 state[4];                                   /* state (ABCD) */
  T_UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  T_UCHAR buffer[64];                         /* input buffer */
} T_MD5_CTX;

#include <rtworks/common.h>


#endif /* _RTWORKS_MD5_H */
