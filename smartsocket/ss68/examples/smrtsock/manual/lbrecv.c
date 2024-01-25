/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/lbrecv.c#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* lbrecv.c - print out contents of messages of type MSG_COUNT */

#include <rtworks/ipc.h>
#define MSG_COUNT 1001

/* ================================================================== */
/*..ProcessMsgCount - callback for processing MSG_COUNT messages */
static void T_ENTRY ProcessMsgCount
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_INT4 field_value;
  
  /* Set current field to first field in message */
  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not access message data.\n");
    return;
  }

  /* Print value from only field */
  if (!TipcMsgNextInt4(msg, &field_value)) {
    TutOut("Could not retrieve message data.\n");
    return;
  }

  TutOut("Message data = %d\n", field_value);

} /* ProcessMsgCount */

/* ==================================================================== */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MT mt;

  /* Set the project name */

  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not find option <project>.\n");
    TutExit(T_EXIT_FAILURE);
  }

  if (!TutOptionSetEnum(option, "smartsockets")) {
    TutOut("Could not set option <project>.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Define a new message type */
  mt = TipcMtCreate("msg_count", MSG_COUNT, "int4");
  if (mt == NULL) {
    TutOut("Could not create message type MSG_COUNT.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Connect to the RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Setup callback to process new message type */
  if (!TipcSrvProcessCbCreate(mt, ProcessMsgCount, NULL)) {
    TutOut("Could not create callback for type MSG_COUNT.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* 
   * Start subscribing to the "/manual/chapter4" subject.
   * If you want to override 
   */
  if (!TipcSrvSubjectSetSubscribe("/manual/chapter4", TRUE)) {
    TutOut("Could not subscribe to subject\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Read and process all incoming messages */
  for (;;) {
    if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)) {
      TutOut("TipcSrvMainLoop failed with error <%s>.\n",
              TutErrStrGet());
    }
  }

} /* main */
