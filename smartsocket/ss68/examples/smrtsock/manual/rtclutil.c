/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtclutil.c#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* rtclutil.c -- RTclient example utilities */

#include <rtworks/ipc.h>
#include "rtclient.h"

/* ================================================================ */
/*..create_ud_msg_types -- create example user-defined msg types */
void create_ud_msg_types()
{
  T_IPC_MT mt;

  /* Create our user-defined message type. */
  mt = TipcMtCreate(EXAMPLE_MT_NAME, EXAMPLE_MT_NUM,
		    EXAMPLE_MT_GRAMMAR);
  if (mt == NULL) {
    TutOut("Could not create example message type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Add the new message type to the DATA logging category. */
  if (!TipcSrvLogAddMt(T_IPC_SRV_LOG_DATA, mt)) {
    TutOut("Could not add example mt to DATA category: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

} /* create_ud_msg_types */
