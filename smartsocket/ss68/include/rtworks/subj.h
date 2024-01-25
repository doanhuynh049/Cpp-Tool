/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/subj.h#1 $
 */

#ifndef _RTWORKS_SUBJ_H_
#define _RTWORKS_SUBJ_H_

/*
 * Structures for hierarchical subject table
 */
typedef struct  T_SUBJECT_TABLE_STRUCT 
                T_SUBJECT_TABLE_STRUCT,
               *T_SUBJECT_TABLE;
typedef struct  T_SUBJECT_ELEMENT_STRUCT 
                T_SUBJECT_ELEMENT_STRUCT,
               *T_SUBJECT_ELEMENT;
typedef struct  T_SUBJECT_TABLE_TRAV_STRUCT
                T_SUBJECT_TABLE_TRAV_STRUCT,
               *T_SUBJECT_TABLE_TRAV;
typedef struct  T_SUBJECT_TABLE_CHANGE_CB_STRUCT 
                T_SUBJECT_TABLE_CHANGE_CB_STRUCT,
               *T_SUBJECT_TABLE_CHANGE_CB;
typedef struct  T_SUBJECT_TABLE_TRAV_STATUS_STRUCT 
                T_SUBJECT_TABLE_TRAV_STATUS_STRUCT,
               *T_SUBJECT_TABLE_TRAV_STATUS;
 
typedef enum {
  T_SUBJECT_TABLE_ITEM_INSERTED,
  T_SUBJECT_TABLE_ITEM_REMOVED
} T_SUBJECT_TABLE_REASON;
 
struct T_SUBJECT_TABLE_TRAV_STATUS_STRUCT {
  T_STR  traversal_subject;
  T_BOOL exact_match;
  T_INT4 potential_matches;
};
 
struct T_SUBJECT_TABLE_CHANGE_CB_STRUCT {
  T_CB                    cb;
  T_SUBJECT_TABLE_REASON  reason;
  T_STR                   subject_name;
  T_PTR                   value; 
};
 
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SUBJECT_TABLE_DESTROY_FUNC)
     T_PROTO((
	      T_PTR,
	      T_PTR
	      ));
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SUBJECT_TABLE_TRAVERSE_FUNC)
     T_PROTO((
	      T_SUBJECT_TABLE_TRAV_STATUS,
	      T_PTR,
	      T_PTR
	      ));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SUBJECT_TABLE_CHANGE_CB_FUNC)
     T_PROTO((
	      T_SUBJECT_TABLE,
	      T_SUBJECT_TABLE_CHANGE_CB,
	      T_CB_ARG
	      ));

#define T_SUBJECT_TABLE_CHANGE_CB_NUM (T_LOBE_IPC_START - 8)

/*
 * The following T_IPC_SUBJECT... defines are actually present in util.h
 */
#define T_SUBJECT_DELIMITER                    T_IPC_SUBJECT_DELIMITER
#define T_SUBJECT_WILDCARD_ONE_LEVEL           T_IPC_SUBJECT_WILDCARD_ONE_LEVEL
#define T_SUBJECT_WILDCARD_MANY_LEVELS         T_IPC_SUBJECT_WILDCARD_MANY_LEVELS

/*
 * Except this one, added in 6.0
 */
#define T_SUBJECT_WILDCARD_MANY_LEVELS_LENGTH  3


#define T_SUBJECT_HAS_WILDCARDS(A)      T_IPC_SUBJECT_HAS_WILDCARDS(A) 
#define T_SUBJECT_IS_ABSOLUTE(A)        T_IPC_SUBJECT_IS_ABSOLUTE(A)
#define T_SUBJECT_IS_DESCENDANT(A, B)   T_IPC_SUBJECT_IS_DESCENDANT(A,B)

#include <rtworks/subjpr.h>

#endif /* _RTWORKS_SUBJ_H_ */

