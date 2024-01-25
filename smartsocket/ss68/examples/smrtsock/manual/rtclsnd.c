/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtclsnd.c#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* rtclsnd.c -- RTclient example sender */

/*
 * This sending RTclient creates its connection and publishes a data 
 * frame of messages to a subject (through RTserver).
 */

#include <rtworks/ipc.h>
#include "rtclient.h"

/* ================================================================ */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MT mt;

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

  /* Log outgoing data messages to a message file. Another */
  /* way to set options is to use TutCommandParseStr. */
  TutCommandParseStr("setopt log_out_data log_out.msg");

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

  TutOut("Publish a frame of data to the receiver's subject.\n");

  TutOut("Publishing a TIME message.\n");
  mt = TipcMtLookupByNum(T_MT_TIME);
  if (mt == NULL) {
    TutOut("Could not look up TIME msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE,
		       T_IPC_FT_REAL8, 1.0,
		       NULL)) {
    TutOut("Could not publish TIME message: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Publishing a NUMERIC_DATA message.\n");
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (mt == NULL) {
    TutOut("Could not look up NUMERIC_DATA msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE,
		       T_IPC_FT_STR, "voltage",
		       T_IPC_FT_REAL8, 33.4534,
		       T_IPC_FT_STR, "switch_pos",
		       T_IPC_FT_REAL8, 0.0,
		       NULL)) {
    TutOut("Could not publish NUMERIC_DATA message: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Publishing an EXAMPLE message.\n");
  mt = TipcMtLookupByNum(EXAMPLE_MT_NUM);
  if (mt == NULL) {
    TutOut("Could not look up EXAMPLE msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE,
		       T_IPC_FT_INT4, 7,
		       T_IPC_FT_STR, "Seven is your lucky number",
		       NULL)) {
    TutOut("Could not publish EXAMPLE message: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Publishing an END_OF_FRAME message.\n");
  mt = TipcMtLookupByNum(T_MT_END_OF_FRAME);
  if (mt == NULL) {
    TutOut("Could not look up END_OF_FRAME msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE, NULL)) {
    TutOut("Could not publish END_OF_FRAME message: error <%s>.\n",
	   TutErrStrGet());
  }

  /* Each RTclient automatically creates a connection process */
  /* callback for CONTROL messages. Use this to send the command */
  /* "quit force" to the receiver's command interface. */
  TutOut("Publishing a CONTROL message to stop the receiver(s).\n");
  mt = TipcMtLookupByNum(T_MT_CONTROL);
  if (mt == NULL) {
    TutOut("Could not look up CONTROL msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE,
		       T_IPC_FT_STR, "quit force",
		       NULL)) {
    TutOut("Could not publish CONTROL message: error <%s>.\n",
	   TutErrStrGet());
  }

  /* Flush the buffered outgoing messages to RTserver. */
  if (!TipcSrvFlush()) {
    TutOut("Could not flush messages to RTserver: error <%s>.\n",
	   TutErrStrGet());
  }

  /* Completely disconnect from RTserver. */
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Could not destroy connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Sender RTclient process exiting successfully.\n");
  return T_EXIT_SUCCESS;	/* all done */
} /* main */
