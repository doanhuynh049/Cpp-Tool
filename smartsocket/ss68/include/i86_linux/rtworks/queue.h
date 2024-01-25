/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/queue.h#1 $
 */

/* KEL - 1995/OCT/13 - Macro error on SASC when no space between   */
/*                     macro def and actual macro statements       */
/* --------------------------------------------------------------- */



/* =================================================================== */
/*                                                                     */
/*                   queue.h -- generic priority queues                */
/*                                                                     */
/* =================================================================== */

/* ----------------------------------------------- */
/* See "Reliable Data Structures" by Plum, p. 7-19 */
/* ----------------------------------------------- */

#ifndef _RTWORKS_QUEUE_H_
#define _RTWORKS_QUEUE_H_

/* ---------------------------------------------------------------------- */
/* Generic queue structure.  Each real queue should have a pointer as the */
/* first element of the structure.                                        */
/* ---------------------------------------------------------------------- */
typedef struct T_Q_NODE T_Q_NODE;
struct T_Q_NODE {
  T_Q_NODE *next;
};

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_Q_CMP_FUNC) 
     T_TYPEDEF((T_Q_NODE *a, T_Q_NODE *b));

/* ----------------------------------- */
/* Macro Interfaces to queue functions */
/* ----------------------------------- */
#define T_Q_P(p) ((T_Q_NODE *) (p))
#define T_Q_PP(p) ((T_Q_NODE **) (p))

#define T_Q_APPEND(fp, rp, p) TutQueueAppend(T_Q_PP(fp), T_Q_PP(rp), T_Q_P(p))
#define T_Q_CLOSE(fp, rp) TutQueueClose(T_Q_PP(fp), T_Q_PP(rp))
#define T_Q_INSERT(fp,rp,p,func) \
  TutQueueInsert(T_Q_PP(fp), T_Q_PP(rp), T_Q_P(p), (T_Q_CMP_FUNC)func)
#define T_Q_LFIND(fp,rp,p,func) \
  TutQueueLeftFind(T_Q_PP(fp), T_Q_PP(rp), T_Q_P(p), (T_Q_CMP_FUNC)func)
#define T_Q_POP(fp, rp) TutQueuePop(T_Q_PP(fp), T_Q_PP(rp))
#define T_Q_PUSH(fp,rp,p) TutQueuePush(T_Q_PP(fp), T_Q_PP(rp), T_Q_P(p))
#define T_Q_R_DETACH(fp,rp,pp)\
  TutQueueRightDetach(T_Q_PP(fp),T_Q_PP(rp),T_Q_PP(pp))
#define T_Q_R_INSERT(fp,rp,p,pp) \
  TutQueueRightInsert(T_Q_PP(fp),T_Q_PP(rp),T_Q_P(p),T_Q_PP(pp))
#define T_Q_DELETE(fp,rp,p) TutQueueDelete(T_Q_PP(fp),T_Q_PP(rp),T_Q_P(p))
#define T_Q_IS_EMPTY(front) ((front) == NULL)
#define T_Q_OPEN(frontp, rearp) (*(frontp) = *(rearp) = NULL)
#define T_Q_TRAVERSE(front,p) for ((p) = front;(p) != NULL;p = (p)->next)
#define T_EACH_Q T_Q_TRAVERSE /* for backwards compatibility */

#endif /* _RTWORKS_QUEUE_H_ */
