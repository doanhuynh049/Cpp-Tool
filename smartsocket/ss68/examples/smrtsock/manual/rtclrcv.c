/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtclrcv.c#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* rtclrcv.c -- RTclient example receiver */

/*
 * The receiving RTclient creates its connection to RTserver, subscribes 
 * to subjects, and receives and processes messages.
 */

#include <rtworks/ipc.h>
#include "rtclient.h"

/* ================================================================ */
/*..cb_process_numeric_data -- process callback for NUMERIC_DATA */
static void T_ENTRY cb_process_numeric_data
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR name;
  T_REAL8 value;

  TutOut("Entering cb_process_numeric_data.\n");

  /* set current field to first field in message */
  if (!TipcMsgSetCurrent(data->msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }

  /* access and print fields */
  while (TipcMsgNextStrReal8(data->msg, &name, &value)) {
    TutOut("%s = %s\n", name, TutRealToStr(value));
  }

  /* make sure we reached the end of the message */
  if (TutErrNumGet() != T_ERR_MSG_EOM) {
    TutOut("Did not reach end of message: error <%s>.\n",
	   TutErrStrGet());
  }
} /* cb_process_numeric_data */

/* ================================================================ */
/*..cb_default -- default callback */
static void T_ENTRY cb_default
(
 T_IPC_CONN conn,
 T_IPC_CONN_DEFAULT_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MT mt;
  T_STR name;

  /* This callback function will be called for messages where */
  /* there are no process callbacks for that message type. */
  TutOut("Entering cb_default.\n");

  /* print out the name of the type of the message */
  if (!TipcMsgGetType(data->msg, &mt)) {
    TutOut("Could not get message type from message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  if (!TipcMtGetName(mt, &name)) {
    TutOut("Could not get name from message type: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  TutOut("Message type name is %s.\n", name);
} /* cb_default */

/* ================================================================ */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MT mt;			/* message type for creating callbacks */

  /* Create user-defined message types. */
  create_ud_msg_types();

  /* Set the option Project to partition ourself. */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not look up option named project: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TutOptionSetEnum(option, EXAMPLE_PROJECT)) {
    TutOut("Could not set option named project: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Allow a command-line argument containing the name of a */
  /* SmartSockets startup command file. This file can be used */
  /* to set options like Server_Names. */
  if (argc == 2) {
    if (!TutCommandParseFile(argv[1])) {
      TutOut("Could not parse startup command file %s: error <%s>.\n",
	     argv[1], TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
  }
  else if (argc != 1) {		/* too many command-line arguments */
    TutOut("Usage: %s [ command_file_name ]\n", argv[0]);
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Creating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* create callbacks to be executed when certain operations occur */
  TutOut("Create callbacks.\n");

  /* process callback for NUMERIC_DATA */
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (mt == NULL) {
    TutOut("Could not look up NUMERIC_DATA msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (TipcSrvProcessCbCreate(mt, cb_process_numeric_data, NULL)
      == NULL) {
    TutOut("Could not create NUMERIC_DATA process cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* default callback */
  if (TipcSrvDefaultCbCreate(cb_default, NULL) == NULL) {
    TutOut("Could not create default cb: error <%s>.\n",
	   TutErrStrGet());
  }
  
  TutOut("Start subscribing to standard subjects.\n");
  if (!TipcSrvStdSubjectSetSubscribe(TRUE, FALSE)) {
    TutOut("Could not subscribe to standard subjects: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Start subscribing to the %s subject.\n", EXAMPLE_SUBJECT);
  if (!TipcSrvSubjectSetSubscribe(EXAMPLE_SUBJECT, TRUE)) {
    TutOut("Could not start subscribing to %s subject: error <%s>.\n",
	   EXAMPLE_SUBJECT, TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* If an error occurs, then TipcSrvMainLoop will restart RTserver */
  /* and return FALSE. We can safely continue. */
  for (;;) {
    if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)) {
      TutOut("TipcSrvMainLoop failed: error <%s>.\n",
	     TutErrStrGet());
    }
  }

  /* This line should not be reached. */
  TutOut("This line should not be reached!!!\n");
  return T_EXIT_FAILURE;
} /* main */
