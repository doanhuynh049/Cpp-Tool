/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson4/snd_umsg.c#1 $
 */

/* 
 * snd_umsg.c - create and send a series of messages of user-defined 
 * type XYZ_COORD_DATA
 */
/* $RTHOME/examples/smrtsock/tutorial/lesson4/snd_umsg.c */

#include <rtworks/ipc.h>
#define XYZ_COORD_DATA 1001

int main(int argc, char **argv)
{
  T_OPTION option;
  T_IPC_MSG msg;
  T_IPC_MT mt;
  T_INT4 i;

  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Define a new message type */
  mt = TipcMtCreate("xyz_coord_data", XYZ_COORD_DATA, 
		    "int4 int4 int4");

  /* Create a message of type XYZ_COORD_DATA */
  msg = TipcMsgCreate(mt);

  /* Assign it the DATA category for logging */
  TipcSrvLogAddMt(T_IPC_SRV_LOG_DATA, mt);

  /* Set the destination subject of the message */
  TipcMsgSetDest(msg, "/tutorial/lesson4");

  for (i = 0; i < 30; i = i + 3) {
    /* 
     * In order to re-use the message, reset the number of the 
     * fields in the message to zero
     */
    TipcMsgSetNumFields(msg, 0);

    /* Build the data part message with 3 integers */
    TipcMsgAppendInt4(msg, i);
    TipcMsgAppendInt4(msg, i+1);
    TipcMsgAppendInt4(msg, i+2);

    /* Send the message */
    TipcSrvMsgSend(msg, TRUE);
    TipcSrvFlush();
  } /* for */

  /* Destroy the message */
  TipcMsgDestroy(msg);

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
