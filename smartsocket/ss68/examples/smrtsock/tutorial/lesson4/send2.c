/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson4/send2.c#1 $
 */

/* send2.c - send a INFO and then NUMERIC_DATA message */
/* $RTHOME/examples/smrtsock/tutorial/lesson4/send2.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_OPTION option;
  T_IPC_MSG msg;

  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Send an INFO message */
  TipcSrvMsgWrite("/tutorial/lesson4", TipcMtLookupByNum(T_MT_INFO), TRUE,
		  T_IPC_FT_STR, "Hello World!", NULL);

  /* Create a message of type NUMERIC_DATA */
  msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_NUMERIC_DATA));

  /* Set the destination subject of the message */
  TipcMsgSetDest(msg, "/tutorial/lesson4");

  /* Build the message with 3 variable-value pairs */
  TipcMsgAppendStrReal8(msg, "X", 10.0);
  TipcMsgAppendStrReal8(msg, "Y", 20.0);
  TipcMsgAppendStrReal8(msg, "Z", 30.0);

  /* Send the message */
  TipcSrvMsgSend(msg, TRUE);
  TipcSrvFlush();

  /* Destroy the message */
  TipcMsgDestroy(msg);

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
