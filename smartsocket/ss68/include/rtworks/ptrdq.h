/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/ptrdq.h#1 $
 */

/* KEL - 1995/OCT/13 - Fix up 80 byte records                      */
/* --------------------------------------------------------------- */



/* =================================================================== */
/*                                                                     */
/*              ptrdq.h  -- generic deques of pointers                 */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_PTR_DEQUE_H_
#define _RTWORKS_PTR_DEQUE_H_

typedef struct t_pdq_node T_PDQ_NODE;
struct t_pdq_node {
  T_PDQ_NODE *left;
  T_PDQ_NODE *right;
  T_PTR ptr;
};

#define T_PDQ_P(p)             ((T_PDQ_NODE *) (p))
#define T_PDQ_PP(p)            ((T_PDQ_NODE **) (p))

#define T_PDQ_EXISTS(pdq, p)   (TutFindPdqNode(T_PDQ_P(pdq), (T_PTR)(p)))

#define T_EACH_PDQ(pdq, pd) \
  for ((pd)=(pdq)->right; (pd)!=pdq; (pd)=(pd)->right)
#define T_EACH_LPDQ(pdq, pd) \
  for ((pd)=(pdq)->left; (pd)!=pdq; (pd)=(pd)->left)

#define T_PDQ_APPEND(pdq, ptr) \
  if ( (pdq)->right == NULL) { \
    T_DQ_OPEN(&pdq); \
  } \
  TutAddPdqNode( (T_PDQ_NODE *)(pdq), (T_PTR)(ptr) )

#define T_PDQ_DELETE(pdq,ptr) \
      TutDeletePdqNode( (T_PDQ_NODE *)(pdq), (T_PTR)(ptr) )

#endif /* _RTWORKS_PTR_DEQUE_H_ */



