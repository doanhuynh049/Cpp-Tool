/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson6/send3.c#1 $
 */

/* send3.c - send a series of MSG_COUNT messages */
/* $RTHOME/examples/smrtsock/tutorial/lesson6/send3.c */

#define MSG_COUNT 2000
#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_INT4 i;

  /* Set the project and unique_subject options */
  TutCommandParseStr("Setopt project smartsockets");    
  TutCommandParseStr("Setopt unique_subject /lesson6/send3");

  /* 
   * Create a new message type which consists of a single integer. We
   * will use it to hold a message count 
   */
  TipcMtCreate("msg_count", MSG_COUNT, "int4");
 
  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  i = 0;
  while (1) {
    i++;
    TutOut("Sending message %d\n", i);
    TipcSrvMsgWrite("/tutorial/lesson6", TipcMtLookupByNum(MSG_COUNT), TRUE,
		    T_IPC_FT_INT4, i, NULL);
    TipcSrvFlush();
    TipcSrvMainLoop(1.0);
  } /* for */

} /* main */
