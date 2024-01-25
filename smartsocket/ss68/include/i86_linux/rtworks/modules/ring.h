/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/modules/ring.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                   ring.h -- generic ring buffers                    */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_RING_H_
#define _RTWORKS_RING_H_

typedef struct T_RB_STRUCT T_RB_STRUCT, *T_RB;
typedef struct T_RB_HEADER_STRUCT T_RB_HEADER_STRUCT, *T_RB_HEADER;
typedef struct T_RB_ENTRY_STRUCT T_RB_ENTRY_STRUCT, *T_RB_ENTRY;
typedef struct T_RB_CONTEXT_STRUCT T_RB_CONTEXT_STRUCT, *T_RB_CONTEXT;

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_RB_ENTRY_TRAV_FUNC)
     T_TYPEDEF((T_RB_ENTRY rbe, T_PTR arg));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_RB_FREE_FUNC) 
     T_TYPEDEF((T_PTR p));

typedef struct T_RING_OVERWRITE_CB_DATA_STRUCT T_RING_OVERWRITE_CB_DATA_STRUCT,
*T_RING_OVERWRITE_CB_DATA;

struct T_RING_OVERWRITE_CB_DATA_STRUCT {
  T_CB cb;
  T_RB_HEADER hdr;
  T_RB_ENTRY rbe;
};

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_RING_OVERWRITE_CB_FUNC) T_TYPEDEF
((T_RB rb,                        /* Ring buffer */
  T_RING_OVERWRITE_CB_DATA data,  /* data */
  T_CB_ARG arg));

#define T_ST_NOT_USED 0          /* This ringbuffer slot is still a virgin */
#define T_ST_UNKNOWN_TIMELESS 4  /* value is unknown and has no time-tag */
#define T_ST_KNOWN_TIMELESS 5    /* value is known but has no time-tag */
#define T_ST_KNOWN_RUNTIME 7     /* Temporal val is known and has a time-tag */
#define T_ST_UNKNOWN_RUNTIME 6   /* value unknown and has a time-tag */

#define T_VALUE_KNOWN(st)   ((st) & 0x1)
#define T_TIME_KNOWN(st)    ((st) & 0x2)

#define T_EACH_RB_ENTRY( rbh, ptr ) for( ptr=TutRingNextEntry(rbh); \
				      ptr != NULL; ptr=TutRingNextEntry(NULL) )

#endif /* _RTWORKS_RING_H_ */
