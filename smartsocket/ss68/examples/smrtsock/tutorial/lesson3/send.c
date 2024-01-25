/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson3/send.c#1 $
 */

/* send.c - send a INFO message with "Hello World!" */
/* $RTHOME/examples/smrtsock/tutorial/lesson3/send.c */

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

  /* Create a message of type INFO */
  msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_INFO));

  /* Set the destination subject of the message */
  TipcMsgSetDest(msg, "/tutorial/lesson3");

  /* Build the message with one string field: "Hello World!" */
  TipcMsgAppendStr(msg, "Hello World!");

  /* Send the message */
  TipcSrvMsgSend(msg, TRUE);
  TipcSrvFlush();

  /* Destroy the message */
  TipcMsgDestroy(msg);

  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
} /* main */
