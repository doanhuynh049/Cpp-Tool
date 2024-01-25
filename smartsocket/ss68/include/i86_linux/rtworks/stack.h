/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/stack.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                     stack.h -- generic stacks                       */
/*                                                                     */
/* =================================================================== */

/* ---------------------------------------------- */
/* See "Reliable Data Structures" by Plum, p. 7-9 */
/* ---------------------------------------------- */

#ifndef _RTWORKS_STACK_H_
#define _RTWORKS_STACK_H_

typedef struct T_ST_NODE T_ST_NODE;
struct T_ST_NODE {
  T_ST_NODE *next;
};

#define T_ST_P(p)             ((T_ST_NODE *) (p))
#define T_ST_PP(p)            ((T_ST_NODE **) (p))
#define T_ST_OPEN(headp)      (*(headp) = NULL)
#define T_ST_CLOSE(h)         (TutStackClose(T_ST_PP(h))) 
#define T_ST_POP(h)           (TutStackPop(T_ST_PP(h)))
#define T_ST_PUSH(h,p)        (TutStackPush(T_ST_PP(h), T_ST_P(p)))
#define T_ST_TRAVERSE(head,p) for ((p) = (head); (p) != NULL; (p) = (p)->next)
#define T_EACH_ST T_ST_TRAVERSE /* for backwards compatibility */
#define T_ST_IS_EMPTY(head)   ((head) == NULL)

#endif /* _RTWORKS_STACK_H_ */

