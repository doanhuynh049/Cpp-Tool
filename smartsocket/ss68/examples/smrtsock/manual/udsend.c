/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/udsend.c#1 $
 */

/* udsend.c - send a series of MSG_COUNT messages */

#define MSG_COUNT 2000
#include <rtworks/ipc.h>

/* ============================================================= */
/*..ProcessMsgCount - callback for processing MSG_COUNT messages */
static void T_ENTRY ProcessMsgCount
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_INT4 msg_count;;

  TipcMsgSetCurrent(msg, 0);
  TipcMsgNextInt4(msg, &msg_count);
  TutOut("...Received message %d\n", msg_count);

} /* ProcessMsgCount */

/* ==================================================================== */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_INT4 i;
  T_IPC_MT mt;
 
  /* Set project */
  TutCommandParseStr("setopt project user_manual");
  TutCommandParseStr("setopt unique_subject send_program");

  /* Create a new message type which consists of a single integer. We
     will use it to hold a message count */
  TipcMtCreate("msg_count", MSG_COUNT, "int4");
 
  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Setup callback to catch MSG_COUNT messages */
  mt = TipcMtLookupByNum(MSG_COUNT);
  TipcSrvProcessCbCreate(mt, ProcessMsgCount, NULL);

  i = 0;
  while (1) {
    i++;
    TutOut("Sending message %d...\n", i);
    TipcSrvMsgWrite("chapter5", TipcMtLookupByNum(MSG_COUNT), TRUE,
		    T_IPC_FT_INT4, i, NULL);
    TipcSrvFlush();
    TipcSrvMainLoop(1.0);
  } /* for */

  return T_EXIT_SUCCESS;
} /* main */
