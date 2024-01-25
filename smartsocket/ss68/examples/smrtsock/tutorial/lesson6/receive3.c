/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson6/receive3.c#1 $
 */

/* receive3.c - sleep and print out contents of MSG_COUNT messages; */
/* $RTHOME/examples/smrtsock/tutorial/lesson6/receive3.c */

#define MSG_COUNT 2000
#include <rtworks/ipc.h>

static void T_ENTRY ProcessMsgCount(T_IPC_CONN conn,
				    T_IPC_CONN_PROCESS_CB_DATA data,
				    T_CB_ARG arg);

/* ========================================================================= */
/*..ProcessMsgCount - callback for processing MSG_COUNT messages */
static void T_ENTRY ProcessMsgCount(conn, data, arg)
T_IPC_CONN conn;
T_IPC_CONN_PROCESS_CB_DATA data;
T_CB_ARG arg;
{
  T_IPC_MSG msg = data->msg;
  T_INT4 msg_count;

  TipcMsgSetCurrent(msg, 0);
  TipcMsgNextInt4(msg, &msg_count);
  TutOut("Message Count = %d\n", msg_count);

} /* ProcessMsgCount */

/* ========================================================================= */
int main(int argc, char **argv)
{
  T_IPC_MT mt;

  /* Set the project and unique_subject options */
  TutCommandParseStr("setopt project smartsockets");
  TutCommandParseStr("setopt unique_subject /lesson6/sleeper");

  /* Define a new message type, MSG_COUNT */
  TipcMtCreate("msg_count", MSG_COUNT, "int4");

  /* Connect to the RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Setup callback to catch MSG_COUNT messages */
  mt = TipcMtLookupByNum(MSG_COUNT);
  TipcSrvProcessCbCreate(mt, ProcessMsgCount, NULL);

  /* Subscribe to the "/tutorial/lesson6" subject */
  TipcSrvSubjectSetSubscribe("/tutorial/lesson6", TRUE);

  /* 
   * Read and process all incoming messages; sleep 10 seconds between
   * processing messages 
   */
  while (1) {
    TipcSrvMainLoop(2.0);
    TutOut("Sleeping...\n");
    TutSleep(10.0);
    TutOut("...Done Sleeping!\n");
  }

} /* main */