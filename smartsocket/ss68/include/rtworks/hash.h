/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/hash.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*               hash.h -- public interface to hash table              */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_HASH_H_
#define _RTWORKS_HASH_H_

/* ------------------------------------------------ */
/* Opaque types not visible outside of util/hash.c. */
/* ------------------------------------------------ */
typedef struct T_HASH_STRUCT T_HASH_STRUCT, *T_HASH, *T_HASH_TABLE;
typedef struct T_HASH_BUCKET_STRUCT T_HASH_BUCKET_STRUCT, *T_HASH_BUCKET;

/* ------------------------------------------------------------- */
/* Define how full to let hash tables get before we resize them. */
/* This number must be a double between 0 and 1.                 */
/* ------------------------------------------------------------- */
#define T_HASH_OVERFLOW (0.75)

/* ------------------------------- */
/* Hash-related function typedefs. */
/* ------------------------------- */
typedef T_UINT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_CALC_FUNC) T_PROTO((T_PTR /*key*/));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_TEST_FUNC) T_PROTO((T_PTR /*key1*/, T_PTR /*key2*/));
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_TRAV_FUNC) T_PROTO((T_PTR /*key*/, 
						    T_PTR /*value*/,
						    T_PTR /*arg*/));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_RESET_FUNC) T_PROTO((T_PTR /*key*/,
						    T_PTR /*value*/,
						    T_PTR /*arg*/));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_DESTROY_FUNC) T_PROTO((T_PTR /*key*/,
						      T_PTR /*value*/,
						      T_PTR /*arg*/));
typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_SORT_CMP_FUNC) T_PROTO((T_PTR /*key1*/,
							 T_PTR /*key2*/));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_SORT_SELECT_FUNC) T_PROTO((T_PTR /*key*/,
							    T_PTR /*value*/));
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_HASH_SORT_TRAV_FUNC) T_PROTO((T_PTR /*value*/,
							 T_PTR /*arg*/));

#endif /* _RTWORKS_HASH_H_ */

