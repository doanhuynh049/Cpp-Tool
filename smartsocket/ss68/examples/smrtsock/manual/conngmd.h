/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/conngmd.h#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* conngmd.h -- common header for connections GMD example */

#include <rtworks/ipc.h>

#define GMD_CONN_NAME "tcp:_node:5252"

void create_conn_cb();
