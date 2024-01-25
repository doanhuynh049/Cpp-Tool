/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/list.h#1 $
 */

/* KEL - 1995/OCT/13 - Fix up 80 byte records                      */
/* --------------------------------------------------------------- */



/* =================================================================== */
/*                                                                     */
/*                   list.h -- generic linked lists                    */
/*                                                                     */
/* =================================================================== */

/* ---------------------------------------------- */
/* See "Reliable Data Structures" by Plum, p. 7-9 */
/* ---------------------------------------------- */

#ifndef _RTWORKS_LIST_H_
#define _RTWORKS_LIST_H_

typedef struct T_LST_NODE T_LST_NODE;
struct T_LST_NODE {
  T_LST_NODE *next;
};

/* ---------------------------------------------------------------- */
/* T_LST_TRAV_DEL_FUNC is for traversing a list to find one node to */
/* delete.  This work, but I'm not entirely happy with it.  I think */
/* it is better to use dequeus then singly-linked lists for this    */
/* purpose.  SMW 11/23/93                                           */
/* ---------------------------------------------------------------- */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LST_TRAV_DEL_FUNC)
     T_TYPEDEF((T_LST_NODE *node, T_PTR arg));

#define T_LST_P(p)             ((T_LST_NODE *) (p))
#define T_LST_PP(p)            ((T_LST_NODE **) (p))

#define T_LST_TRAVERSE(head,p) for ((p) = (head); (p) != NULL; (p) = (p)->next)
#define T_LST_IS_EMPTY(head) ((head) == NULL)
#define T_LST_LENGTH(p) TutListLength(T_LST_P(p))
#define T_LST_DELETE(headp, p) TutListDelete(T_LST_PP(headp), T_LST_P(p))
#define T_LST_TRAV_DEL(headp,func,arg) \
           TutListTravDel(T_LST_PP(headp), func, arg)
#define T_LST_END(head) TutListEnd(T_LST_P(head))
#define T_LST_POS(head, p) TutListPos(T_LST_P(head), T_LST_P(p))
/* This is kinda cheating, but still useful.  SMW 11/03/93 */
#define T_LST_CLOSE(headp) (TutStackClose(T_ST_PP(headp)))

#endif /* _RTWORKS_LIST_H_ */
