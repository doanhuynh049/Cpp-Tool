/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson2/send.c#1 $
 */

/* send.c - send INFO message to /tutorial/lesson2 subject */
/* $RTHOME/examples/smrtsock/tutorial/lesson2/send.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_OPTION option;

  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to RTserver */
  TipcSrvCreate(T_IPC_SRV_CONN_FULL);

  /* Send the message to the "/tutorial/lesson2" subject */
  TipcSrvMsgWrite("/tutorial/lesson2", 
		  TipcMtLookupByNum(T_MT_INFO), TRUE,
                  T_IPC_FT_STR, "Hello World!", NULL);
  TipcSrvFlush();

  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
