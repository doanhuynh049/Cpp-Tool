/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/bppq.h#1 $
 */

/* --------------------------------------------------------------------- */
/*               bppq.h -- boundaried priority queues                    */
/*                    based on ptrq.h/ptrqueue.c                         */
/*                                                                       */
/* A boundaried priority pointer queue, or bppq (pronounced 'bopPQ') is  */
/* like a generic priority queue, except that it supports a 'boundary    */
/* insert'. Basically, insertions are not permitted to be re-ordered     */
/* across a boundary. From the insertion standpoint, the queue becomes   */
/* empty after a boundary insert (reads will still see all items).       */
/* --------------------------------------------------------------------- */

#ifndef _RTWORKS_BOUNDED_PRIORITY_PTR_QUEUE_H_
#define _RTWORKS_BOUNDED_PRIORITY_PTR_QUEUE_H_

typedef struct T_BPPQ_HEADER T_BPPQ_HEADER;
struct T_BPPQ_HEADER {
  T_PQ_NODE *head;
  T_PQ_NODE *tail;
  T_PQ_NODE *body; /* last for compatibility with T_PQ_HEADER, just in case */
  T_INT4    boundary;
  T_INT4    count;
};

/* ----------------------------------- */
/* Macro Interfaces to queue functions */
/* ----------------------------------- */
#define T_BPPQ_P(p)                 ((T_PQ_NODE *)(p))
#define T_BPPQ_PP(p)                ((T_PQ_NODE **)(p))
#define T_BPPQ_H(p)                 ((T_BPPQ_HEADER *)(p))

#define T_BPPQ_NEXT(p)              ((p)->next)


#define T_BPPQ_CLOSE(pqh)           TutBppqClose(T_BPPQ_H(pqh))
#define T_BPPQA_CLOSE(pqh)          T_BPPQ_CLOSE(&(pqh))

#define T_BPPQA_OPEN(pqh) \
{ \
  pqh.head = pqh.tail = pqh.body = NULL; \
  pqh.boundary = -1; pqh.count = 0; \
}
#define T_BPPQA_MOVE(pqha,pqhb) \
{ \
  pqhb.head = pqha.head; \
  pqhb.body = pqha.body; \
  pqhb.tail = pqha.tail; \
  pqhb.count = pqha.count; \
  pqhb.boundary = pqha.boundary; \
  T_BPPQA_OPEN(pqha) \
}

#define T_BPPQ_OPEN(pqh)            T_BPPQA_OPEN((*(pqh)))
#define T_BPPQ_MOVE(pqha,pqhb)      T_BPPQA_MOVE((*(pqha)),(*(pqhb)))


#define T_BPPQ_HEAD(pqh)            ((pqh)->head)
#define T_BPPQ_BODY(pqh)            ((pqh)->body)
#define T_BPPQ_TAIL(pqh)            ((pqh)->tail)


#define T_BPPQ_BAPPEND(pqh,p)       TutBppqBAppend(T_BPPQ_H(pqh), (T_PTR)(p))
#define T_BPPQ_NINSERT(pqh,p,v)     TutBppqNInsert(T_BPPQ_H(pqh), (T_PTR)(p), (v))
#define T_BPPQ_INSERT(pqh,p,func)   TutBppqInsert(T_BPPQ_H(pqh), (T_PTR)(p), \
                                                  (T_PQ_CMP_FUNC)(func))
#define T_BPPQ_PUSH(pqh,p)          T_BPPQ_NINSERT(pqh, p, 0)
#define T_BPPQ_POP(pqh)             TutBppqPop(T_BPPQ_H(pqh))
#define T_BPPQ_DELETE(pqh,p,pos)    TutBppqDelete(T_BPPQ_H(pqh),(T_PTR)(p),(pos))

#define T_BPPQ_IS_EMPTY(pqh)        (T_BPPQ_HEAD(pqh) == NULL)
#define T_BPPQ_TRAVERSE(pqh,p)      for((p)=(pqh)->head;(p)!=NULL;(p)=(p)->next)
#define T_BPPQ_COUNT(pqh)           ((pqh)->count)


#define T_BPPQA_HEAD(pqh)           (pqh.head)
#define T_BPPQA_BODY(pqh)           (pqh.body)
#define T_BPPQA_TAIL(pqh)           (pqh.tail)

#define T_BPPQA_BAPPEND(pqh,p)      T_BPPQ_BAPPEND(&(pqh), p)
#define T_BPPQA_NINSERT(pqh,p,v)    T_BPPQ_NINSERT(&(pqh), p, v)
#define T_BPPQA_INSERT(pqh,p,func)  T_BPPQ_INSERT(&(pqh), p, func)
#define T_BPPQA_PUSH(pqh,p)         T_BPPQ_PUSH(&(pqh), p)
#define T_BPPQA_POP(pqh)            T_BPPQ_POP(&(pqh))
#define T_BPPQA_DELETE(pqh,p,pos)   T_BPPQ_DELETE(&(pqh),p,pos)

#define T_BPPQA_IS_EMPTY(pqh)       T_BPPQ_IS_EMPTY(&(pqh))
#define T_BPPQA_TRAVERSE(pqh,p)     for((p)=pqh.head;(p)!=NULL;(p)=(p)->next)
#define T_BPPQA_COUNT(pqh)          (pqh.count)

#endif /* _RTWORKS_BOUNDED_PRIORITY_PTR_QUEUE_H_ */
