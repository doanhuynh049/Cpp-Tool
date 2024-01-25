/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtclient.h#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* rtclient.h -- common header for RTclient examples */

#define EXAMPLE_PROJECT "example"
#define EXAMPLE_SUBJECT "rcv"
#define EXAMPLE_MT_NAME "example_mt"
#define EXAMPLE_MT_NUM 42
#define EXAMPLE_MT_GRAMMAR "int4 /*code*/ str /*explanation*/"

void create_ud_msg_types();
