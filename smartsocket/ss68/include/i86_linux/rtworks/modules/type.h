/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/modules/type.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                    type.h -- basic type information                 */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_TYPE_H_
#define _RTWORKS_TYPE_H_

/* ---------------------------------------------------------------- */
/* If you change, add, or delete any of these defines, you should   */
/* also update the function TutTypeToString in util.c.  SMW 9/20/89 */
/* ---------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* To keep type information, we use a bit mask for storing type           */
/* information.  I guess that if we get more than 32 different types, we  */
/* are in trouble, but by then we might have some other mechanism for     */
/* types.  I guess a promising place to look would be the GNU C compiler, */
/* or the AT&T C++ translator.  SMW 9/21/89                               */
/* ---------------------------------------------------------------------- */
typedef T_UINT4 T_TYPE;

/* ------------------------------------------- */
/* T_TYPE_VOID MUST have all bits cleared (0). */
/* ------------------------------------------- */
#define T_TYPE_VOID         0 /* no type */

/* ------------ */
/* scalar types */
/* ------------ */
#define T_TYPE_BOOLEAN      1 /* true or false */
#define T_TYPE_ENUM         2 /* user defined values */
#define T_TYPE_NUMERIC      4 /* floating-point */
#define T_TYPE_STRING       8 /* array of chars */

/* --------------------- */
/* sets -- unimplemented */
/* --------------------- */
#define T_TYPE_SET_ENUM     16 /* enumerated set */
#define T_TYPE_SET_NUMERIC  32 /* numeric set */
#define T_TYPE_SET_STRING   64 /* string set */

/* ---------------------- */
/* lists -- unimplemented */
/* ---------------------- */
#define T_TYPE_LIST_ENUM    128
#define T_TYPE_LIST_NUMERIC 256
#define T_TYPE_LIST_STRING  512

/* -------------------------------------------------------- */
/* T_TYPE_ANY MUST be the bitwise OR of all the above types */
/* -------------------------------------------------------- */
#define T_TYPE_ANY          1023 /* any of basic types */

/* ------------------------------------------------------------------ */
/* T_TYPE_FIRST_ARG is used by the external functions to specify that */
/* the return type of a function is the same as the type of the       */
/* first argument.                                                    */
/* ------------------------------------------------------------------ */
#define T_TYPE_FIRST_ARG    1024 /* use type of first argument to function */

#define T_TYPE_SLOT         2048 /* argument must be a slot */

/* ------------------------------------------------- */
/* These types are used by the generic ring buffers. */
/* ------------------------------------------------- */
#define T_TYPE_NUMERIC_FLOAT 4096
#define T_TYPE_NUMERIC_DOUBLE T_TYPE_NUMERIC
#define T_TYPE_INT           8192

/* -------------------------------------------------------------------- */
/* T_TYPE_MAX_VALUE is used for error checking.  It should be the value */
/* produced when all the above defines are OR'ed together.              */
/* -------------------------------------------------------------------- */
#define T_TYPE_MAX_VALUE    16383 /* biggest number used */


#endif /* _RTWORKS_TYPE_H_ */
