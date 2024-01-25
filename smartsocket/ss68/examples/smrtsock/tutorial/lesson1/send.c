/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson1/send.c#1 $
 */

/* send.c - send an INFO message to /tutorial/lesson1 subject */
/* $RTHOME/examples/smrtsock/tutorial/lesson1/send.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  /*
   * This one function call will perform the following tasks:
   *    1. Since there was no explicit call to connect to RTserver,
   *       the process will first establish a connection to RTserver
   *       (this might including auto-starting RTserver if it is not
   *       already running)
   *    2. A message of type INFO will be created.
   *    3. The data part of the message will contain the text "Hello World!".
   *    4. The message will be sent to the RTserver process to be delivered to 
   *       the subject (topic) "/tutorial/lesson1"
   *    5. The message will then be destroyed.
   */
  TipcSrvMsgWrite("/tutorial/lesson1", 
		  TipcMtLookupByNum(T_MT_INFO), TRUE, 
   		  T_IPC_FT_STR, "Hello World!", NULL);

  /* This call is necessary to ensure the message is sent immediately */
  TipcSrvFlush();

  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
} /* main */
