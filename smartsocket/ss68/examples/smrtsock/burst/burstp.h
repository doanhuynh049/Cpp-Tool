/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/burst/burstp.h#1 $
 */

#include "burst.h"

#ifndef _RTWORKS_BURST_PRIVATE_H
#define _RTWORKS_BURST_PRIVATE_H

#define T_MT_BURST 9000

struct T_IPC_BURST_STRUCT {
  T_INT4        auto_flush_count;      /* size of burst */
  T_IPC_MSG     msg;                   /* message to hold the large burst message */
  T_INT4*       info_field_array;      /* minimal info about a message */
  T_INT4        info_field_size;       /* size of the info_field_array */
  T_HASH_TABLE  ipc_burst_table;       /* hash table to hold pointers to new bursts */
};

#endif /* _RTWORKS_BURST_PRIVATE_H */
