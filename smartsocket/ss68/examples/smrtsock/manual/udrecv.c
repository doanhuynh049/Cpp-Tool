/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/udrecv.c#1 $
 */

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
  TipcSrvMsgWrite("send_program", TipcMtLookupByNum(MSG_COUNT), TRUE,
		   T_IPC_FT_INT4, msg_count, NULL);
  TipcSrvFlush();

} /* ProcessMsgCount */

/* ==================================================================== */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_IPC_MT mt;
 
  if (argc != 2) {
    /* Too many command line args */
    TutOut("Usage: %s [ command_file_name ]\n", argv[0]);
    TutExit(T_EXIT_FAILURE);
  }

  TipcInitCommands();

  /* Set project */
  TutCommandParseStr("setopt project user_manual");

  if (!TutCommandParseFile(argv[1])) {
    TutOut("udrecv: could not parse command file %s.\n", argv[1]);
    TutExit(T_EXIT_FAILURE);
  }

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

  /* Subscribe to standard subjects */
  TipcSrvStdSubjectSetSubscribe(TRUE, TRUE);

  /* Sunscribe to the "chapter5" subject */
  TipcSrvSubjectSetSubscribe("chapter5", TRUE);

  /* Read and process all incoming messages */
  while (1) {
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);
  }

  return T_EXIT_SUCCESS;
} /* main */
