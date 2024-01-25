/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson3/send2.c#1 $
 */

/* send2.c - send a NUMERIC_DATA message with 3 variables set */
/* $RTHOME/examples/smrtsock/tutorial/lesson3/send2.c */

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

  /* Create a message of type NUMERIC_DATA */
  msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_NUMERIC_DATA));

  /* Set the destination subject of the message */
  TipcMsgSetDest(msg, "/tutorial/lesson3");

  /* 
   * Build a NUMERIC_DATA message with 3 variable-value pairs, 
   * set as follows: (X, 10.0), (Y, 20.0), and (Z, 30.0)
   */
  TipcMsgAppendStr(msg, "X");
  TipcMsgAppendReal8(msg, 10.0);  
  TipcMsgAppendStr(msg, "Y");
  TipcMsgAppendReal8(msg, 20.0);  
  TipcMsgAppendStr(msg, "Z");
  TipcMsgAppendReal8(msg, 30.0);

  /* Send the message */
  TipcSrvMsgSend(msg, TRUE);
  TipcSrvFlush();

  /* Destroy the message */
  TipcMsgDestroy(msg);

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */





