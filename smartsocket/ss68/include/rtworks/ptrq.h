/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/ptrq.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                ptrq.h -- generic priority queues                    */
/*                                                                     */
/* =================================================================== */

/* ----------------------------------------------- */
/* See "Reliable Data Structures" by Plum, p. 7-19 */
/* ----------------------------------------------- */

#ifndef _RTWORKS_PTR_QUEUE_H_
#define _RTWORKS_PTR_QUEUE_H_

/* --------------------------------------------------------------------- */
/* This is the generic pointer queue node.  Users of these macros should */
/* define their own data structures (which the macros will then cast to  */
/* a T_PQ_NODE.                                                          */
/* --------------------------------------------------------------------- */
typedef struct T_PQ_NODE T_PQ_NODE;
struct T_PQ_NODE {
  T_PQ_NODE *next;
  T_PTR ptr;
};

typedef struct T_PQ_HEADER T_PQ_HEADER;
struct T_PQ_HEADER {
  T_PQ_NODE *front;
  T_PQ_NODE *rear;
};

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_PQ_CMP_FUNC) 
     T_TYPEDEF((T_PTR a, T_PTR b));

/* ----------------------------------- */
/* Macro Interfaces to queue functions */
/* ----------------------------------- */
#define T_PQ_P(p)               ((T_PQ_NODE *)(p))
#define T_PQ_PP(p)              ((T_PQ_NODE **)(p))
#define T_PQ_H(p)		((T_PQ_HEADER *)(p))
#define T_PQ_FRONT(pqh)         ((pqh)->front)
#define T_PQ_REAR(pqh)          ((pqh)->rear)

#define T_PQ_POP(pqh)           TutPqPop(T_PQ_H((pqh)))
#define T_PQ_ADD(pqh,p)	        TutPqAdd(T_PQ_H((pqh)), (T_PTR)(p))
#define T_PQ_DELETE(pqh,p)      TutPqDelete(T_PQ_H((pqh)), (T_PTR)(p))
#define T_PQ_EXISTS(pqh,p)      (TutPqFind(T_PQ_H(pqh), (T_PTR)(p)) != NULL)
#define T_PQ_OPEN(pqh)  	(T_PQ_FRONT(pqh) = T_PQ_REAR(pqh) = NULL)
#define T_PQ_NEXT(p)            ((p)->next)
#define T_PQ_CLOSE(pqh)         TutPqClose(T_PQ_H(pqh))
#define T_PQ_PUSH(pqh,p)        TutPqPush(T_PQ_H(pqh), (T_PTR)(p))
#define T_PQ_INSERT(pqh,p,func) \
      TutPqInsert(T_PQ_H(pqh), (T_PTR)(p), (T_PQ_CMP_FUNC)(func))
#define T_PQ_RINSERT(pqh,v,p)  TutPqRInsert(T_PQ_H(pqh), (T_PTR)(v), T_PQ_P(p))
#define T_PQ_IS_EMPTY(pqh)      (T_PQ_FRONT(pqh) == NULL)
#define T_PQ_TRAVERSE(pqh,p)    for ((p)=(pqh)->front;(p) != NULL;p=(p)->next)
#define T_EACH_PQ T_PQ_TRAVERSE /* backwards compatibility */
#define T_PQ_COUNT(pqh)	        TutPqCount(T_PQ_H (pqh))

#define T_PQA_FRONT(pqh)        T_PQ_FRONT(&pqh)
#define T_PQA_REAR(pqh)         T_PQ_REAR(&pqh)
#define T_PQA_POP(pqh)          T_PQ_POP(T_PQ_H((&pqh)))
#define T_PQA_ADD(pqh,p)        T_PQ_ADD(&pqh, p)
#define T_PQA_DELETE(pqh,p)     T_PQ_DELETE(&pqh, p)
#define T_PQA_EXISTS(pqh,p)     T_PQ_EXISTS(&pqh, p)
#define T_PQA_OPEN(pqh)         T_PQ_OPEN(&pqh)
#define T_PQA_CLOSE(pqh)        T_PQ_CLOSE(&pqh)
#define T_PQA_PUSH(pqh,p)       T_PQ_PUSH(&pqh, p)
#define T_PQA_INSERT(pqh,p,func) T_PQ_INSERT(&pqh, p, func)
#define T_PQA_RINSERT(pqh,v,p)  T_PQ_RINSERT(&pqh, v, p)
#define T_PQA_IS_EMPTY(pqh)     T_PQ_IS_EMPTY(&pqh)
#define T_PQA_TRAVERSE(pqh,p)   for((p)=pqh.front;(p)!=NULL;(p)=(p)->next)
#define T_EACH_PQA T_PQA_TRAVERSE /* backwards compatibility */
#define T_PQA_COUNT(pqh)        T_PQ_COUNT(&pqh)

#endif /* _RTWORKS_PTR_QUEUE_H_ */
