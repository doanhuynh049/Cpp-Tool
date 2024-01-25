/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/shrq.h#1 $
 */

/*
 * ShrQ's (or Shared Queues, pronounced "sharks") are a simple FIFO queue
 * structure designed to be shared by multiple threads with one reader and one
 * or more writers. They minimize lock contention by implementing a dual queue.
 * Writers append to the "in" queue, while the single reader reads from the
 * "out" queue. Assuming the structure itself is protected, the reader need
 * only obtain a lock when the "out" queue is empty. When this happens, the
 * queue's lock is acquired by the writer and the "in" and "out" queues are
 * swapped.
 *
 * WARNING: For shared queues to work correctly, the SHRQ_ITEM structure MUST
 * either be the first item in the 'real' queue item structure, or must be
 * inlined EXACTLY into the real item structure. For ease-of-modification, the
 * former technique is reccommended.
 *
 * WARNING: The T_SHRQ_ITEM_STRUCT structure definition is critical to the
 * proper function of the assembly shrq functions and SHOULD NOT BE MODIFIED.
 * If it is necessary to modify them, then it is CRITICAL that the assembly
 * code which manipulates them be corrected to reflect the new offsets.
 */

#ifndef  _RTWORKS_SHRQ_H_
#define  _RTWORKS_SHRQ_H_

/* DO NOT modify this structure - See above WARNING */
typedef struct T_SHRQ_ITEM_STRUCT {
    struct T_SHRQ_ITEM_STRUCT * next; /* MUST BE FIRST! - &next == &item */
} T_SHRQ_ITEM_STRUCT, *T_SHRQ_ITEM;

typedef struct T_SHRQ_STRUCT
               T_SHRQ_STRUCT,
              *T_SHRQ;

#endif /* _RTWORKS_SHRQ_H_ */
