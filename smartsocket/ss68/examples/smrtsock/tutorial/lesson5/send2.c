/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson5/send2.c#1 $
 */

/* send2.c - send a series of INFO messages */
/* $RTHOME/examples/smrtsock/tutorial/lesson5/send2.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_OPTION option;

  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  TutCommandParseStr("setopt unique_subject /lesson5/sender");

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  while(1) {
    TipcSrvMsgWrite("/tutorial/lesson5", TipcMtLookupByNum(T_MT_INFO), TRUE,
		    T_IPC_FT_STR, "Hello World!", NULL);
    TipcSrvFlush();
    TipcSrvMainLoop(1.0);
  } /* while */

} /* main */
