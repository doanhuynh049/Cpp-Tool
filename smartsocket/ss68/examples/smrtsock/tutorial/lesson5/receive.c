/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson5/receive.c#1 $
 */

/* receive.c - print out contents of INFO messages*/
/* $RTHOME/examples/smrtsock/tutorial/lesson5/receive.c */

#include <rtworks/ipc.h>

static void T_ENTRY ProcessInfo(T_IPC_CONN conn, 
				T_IPC_CONN_PROCESS_CB_DATA data, 
				T_CB_ARG arg);
static void T_ENTRY DefaultReceiveFunc(T_IPC_CONN conn, 
				       T_IPC_CONN_PROCESS_CB_DATA data, 
				       T_CB_ARG arg);

/* ========================================================================= */
/*..ProcessInfo --- callback for processing INFO messages */
static void T_ENTRY ProcessInfo(conn, data, arg)
T_IPC_CONN conn;
T_IPC_CONN_PROCESS_CB_DATA data;
T_CB_ARG arg;
{
  T_IPC_MSG msg = data->msg;
  T_STR MsgText;

  TipcMsgSetCurrent(msg, 0);
  TipcMsgNextStr(msg, &MsgText);
  TutOut("Text from INFO message = %s\n", MsgText);

} /* ProcessInfo */

/* ========================================================================= */
/*..DefaultReceiveFunc -- default callback */
static void T_ENTRY DefaultReceiveFunc(conn, data, arg)
T_IPC_CONN conn;
T_IPC_CONN_PROCESS_CB_DATA data;
T_CB_ARG arg;
{
  T_IPC_MSG msg = data->msg;
  T_IPC_MT mt;
  T_STR name;

  /* print out the name of the type of the message */
  TipcMsgGetType(msg, &mt);
  TipcMtGetName(mt, &name);
  TutOut("Receive: unexpected message type name is <%s>\n", name);

} /* DefaultReceiveFunc */

/* ========================================================================= */
int main(int argc, char **argv)
{
  T_IPC_MT mt;

  /* Set the project name */
  TutCommandParseStr("setopt project smartsockets");

  /* Connect to the RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Setup callback to catch INFO messages */
  mt = TipcMtLookupByNum(T_MT_INFO);
  TipcSrvProcessCbCreate(mt, ProcessInfo, NULL);

  /* Setup default callback to process anything but INFO messages */
   TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);

  /* Start subscribing to the "/tutorial/lesson5" subject */
  TipcSrvSubjectSetSubscribe("/tutorial/lesson5", TRUE);

  /* Read and Process all incoming messages */
  TipcSrvMainLoop(T_TIMEOUT_FOREVER);

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
