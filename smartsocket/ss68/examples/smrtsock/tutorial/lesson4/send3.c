/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson4/send3.c#1 $
 */

/* send3.c - send a INFO and then a series of NUMERIC_DATA messages */
/* $RTHOME/examples/smrtsock/tutorial/lesson4/send3.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_OPTION option;
  T_IPC_MSG msg;
  T_INT4 i;

  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Send an INFO message */
  TipcSrvMsgWrite("/tutorial/lesson4", 
		  TipcMtLookupByNum(T_MT_INFO), TRUE,
		  T_IPC_FT_STR, "Hello World!", NULL);

  /* Create a message of type NUMERIC_DATA */
  msg = TipcMsgCreate( TipcMtLookupByNum(T_MT_NUMERIC_DATA) );

  /* Set the destination subject of the message */
  TipcMsgSetDest(msg, "/tutorial/lesson4");

  /* Each time through the loop send a NUMERIC_DATA message with 3 values */
  for (i = 0; i < 30; i = i + 3) {
    /* 
     * In order to re-use the message, reset the number of the 
     * fields in the message to zero 
     */
    TipcMsgSetNumFields(msg, 0);

    /* Build the message with 3 variable-value pairs */
    TipcMsgAppendStrReal8(msg, "X", (T_REAL8)i);
    TipcMsgAppendStrReal8(msg, "Y", (T_REAL8)i+1);
    TipcMsgAppendStrReal8(msg, "Z", (T_REAL8)i+2);

    /* Send the message */
    TipcSrvMsgSend(msg, TRUE);
    TipcSrvFlush();
  } /* for */

  /* Destroy the message */
  TipcMsgDestroy(msg);
	
  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
