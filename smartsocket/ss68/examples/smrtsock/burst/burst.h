/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/burst/burst.h#1 $
 */

#include <rtworks/ipc.h>

#ifndef _RTWORKS_BURST_H
#define _RTWORKS_BURST_H

/*
 * Burst filter function
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_BURST_FILTER_FUNC)
T_TYPEDEF
((
 T_IPC_MSG msg,
 T_PTR arg
));

typedef T_BURST_FILTER_FUNC T_BURST_MSG_TRAV_FUNC;
typedef struct T_IPC_BURST_STRUCT T_IPC_BURST_STRUCT, *T_IPC_BURST;

#include "burstpr.h"

#endif /* _RTWORKS_BURST_H */
