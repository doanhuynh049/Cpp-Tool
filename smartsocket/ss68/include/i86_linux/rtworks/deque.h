/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/deque.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                  deque.h -- double ended queues                     */
/*                                                                     */
/* =================================================================== */

/* --------------------------------------- */
/* See "Reliable Data Structures" by Plum. */
/* --------------------------------------- */

#ifndef _RTWORKS_DEQUE_H_
#define _RTWORKS_DEQUE_H_

typedef struct T_DQ_NODE T_DQ_NODE;
struct T_DQ_NODE {
  T_DQ_NODE *left;
  T_DQ_NODE *right;
};

#define T_DQ_P(p)        ((T_DQ_NODE *) (p))
#define T_DQ_PP(p)       ((T_DQ_NODE **) (p))

/* ----------- */
/* pure macros */
/* ----------- */
#define T_DQ_APPEND(dq, d)   T_DQ_R_INSERT(d, (dq)->left)
#define T_DQ_IS_EMPTY(dq)    (T_DQ_P(dq)->right == (T_DQ_P(dq)))
#define T_DQ_L_DETACH(dq, d) T_DQ_DETACH(T_DQ_P(dq), T_DQ_P(d)->left)
#define T_DQ_L_INSERT(d, q)  T_DQ_R_INSERT((d), (q)->left)
#define T_DQ_PRED(dq, d) \
  (T_DQ_P(d)->left == T_DQ_P(d) ? T_NULL : (d)->left)
#define T_DQ_PRED_WRAP(dq, d) \
  (T_DQ_P(d)->left == T_DQ_P(d) ? (dq)->left : (d)->left)
#define T_DQ_PUSH(dq, d)     T_DQ_R_INSERT((d), (dq))
#define T_DQ_R_DETACH(d)     T_DQ_DETACH(T_DQ_P(d)->right)
#define T_DQ_NEXT(dq, d) \
  (T_DQ_P(d)->right == T_DQ_P(dq) ? T_NULL : (d)->right)
#define T_DQ_NEXT_WRAP(dq, d) \
  (T_DQ_P(d)->right == T_DQ_P(dq) ? (dq)->right : (d)->right)
#define T_DQ_TRAVERSE(dq, d) for (d = (dq)->right; (d) != dq; d = (d)->right)
#define T_EACH_DQ T_DQ_TRAVERSE /* backwards compatibility */
#define T_DQ_REMOVE(d)				\
if (T_DQ_P(d)->left != T_DQ_P(d)) {		\
  T_DQ_P(d)->right->left = T_DQ_P(d)->left;	\
  T_DQ_P(d)->left->right = T_DQ_P(d)->right;	\
  T_DQ_P(d)->left = T_DQ_P(d)->right = T_NULL;		\
}
#define T_DQ_MIGRATE(dq1, dq2) \
T_DQ_P(dq1)->right->left = T_DQ_P(dq2); \
T_DQ_P(dq1)->left->right = T_DQ_P(dq2); \
T_DQ_P(dq2)->right = T_DQ_P(dq1)->right; \
T_DQ_P(dq2)->left = T_DQ_P(dq1)->left; \
T_DQ_P(dq1)->left = T_DQ_P(dq1)->right = T_DQ_P(dq1);

/* ------------------------- */
/* function-interface macros */
/* ------------------------- */
#define T_DQ_CLOSE(pdq)      TutDqClose(T_DQ_PP(pdq))
#define T_DQ_DETACH(dq, d)   TutDqDetach(T_DQ_P(dq), T_DQ_P(d))
#define T_DQ_FIRST(dq)       TutDqFirst(T_DQ_P(dq))
#define T_DQ_LAST(dq)        TutDqLast(T_DQ_P(dq))
#define T_DQ_OPEN(pdq)       TutDqOpen(T_DQ_PP(pdq))
#define T_DQ_POP(dq)         TutDqPop(T_DQ_P(dq))
#define T_DQ_R_INSERT(d, p)  TutDqRightInsert(T_DQ_P(d), T_DQ_P(p))

#endif /* _RTWORKS_DEQUE_H_ */
