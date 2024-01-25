/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/ptrstk.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*              ptrstk.h -- generic stacks of pointers                 */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_PTR_STACK_H_
#define _RTWORKS_PTR_STACK_H_

typedef struct T_PST_NODE T_PST_NODE;
struct T_PST_NODE {
  T_PST_NODE *next;
  char *ptr; /* should really be void, but MIPS C compiler is broken */
};

#define T_PST_P(p)           ((T_PST_NODE *) (p))
#define T_PST_PP(p)          ((T_PST_NODE **) (p))
#define T_PST_TRAVERSE(head, p)  for ((p)=(head); (p) != NULL; (p) = (p)->next)
#define T_EACH_PST T_PST_TRAVERSE /* for backwards compatibility */
#define T_PST_PUSH(h, p)     TutPushPstNode(T_PST_PP(h), (char *)(p))
#define T_PST_POP(h)         TutPopPstNode(T_PST_PP(h))
#define T_PST_FIND(head, p)  TutFindPstNode(T_PST_P(head), p)

#endif /* _RTWORKS_PTR_STACK_H_ */
