/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtclgmds.c#1 $
 */

/* 
 * Mentioned in "RTserver and RTclient" chapter in the 
 * "SmartSockets User's Guide". 
 */

/* rtclgmds.c -- RTclient GMD example sender */

/*
 * This RTclient illustrates how most possible GMD failures can occur.
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
  T_IPC_MT mt; /* message type for sending messages */
  
  /* Configure sender to use file-based GMD. */
  TutCommandParseStr("setopt unique_subject gmd_sender");
  
  /* Set the option Application to partition ourself. */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not lookup the option named project.\n");
    TutExit(T_EXIT_FAILURE);
  }
  if (!TutOptionSetEnum(option, EXAMPLE_PROJECT)) {
    TutOut("Could not set the option named project.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Allow a command-line argument containing the name of an */
  /* RTworks startup command file. This file can be used to set */
  /* options like Server_Names. */
  if (argc == 2) {
    if (!TutCommandParseFile(argv[1])) {
      TutOut("Could not parse startup command file %s.\n", argv[1]);
      TutExit(T_EXIT_FAILURE);
    }
  }
  else if (argc != 1) { /* too many command-line arguments */
    TutOut("Usage: %s [ command_file_name ]\n", argv[0]);
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Delete old GMD files.\n");
  if (!TipcSrvGmdFileDelete()) {
    TutOut("Could not delete old GMD files.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Creating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create the connection to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Start receiving standard subjects.\n");
  if (!TipcSrvStdSubjectSetSubscribe(TRUE, FALSE)) {
    TutOut("Could not start receiving standard subjects.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Start receiving the %s subject.\n", EXAMPLE_SUBJECT);
  if (!TipcSrvSubjectSetSubscribe(EXAMPLE_SUBJECT, TRUE)) {
    TutOut("Could not start receiving %s subject.\n", EXAMPLE_SUBJECT);
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Send a message with GMD to both processes.\n");
  TutOut("Receiver will cause a T_ERR_GMD_RECEIVER_EXIT GMD failure.\n");
  mt = TipcMtLookupByNum(T_MT_CONTROL);
  if (mt == NULL) {
    TutOut("Could not lookup CONTROL message type.\n");
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE,
		       T_IPC_PROP_DELIVERY_MODE, T_IPC_DELIVERY_ALL,
		       T_IPC_FT_STR, "echo Got GMD message #1.",
		       NULL)) {
    TutOut("Could not send CONTROL message.\n");
  }
  
  /* Give receiver time to "crash" and restart. */
  TutOut("Read data to allow acks/nacks time to come in.\n");
  if (!TipcSrvMainLoop(5.0)) {
    TutOut("Could not read from RTserver.\n");
  }
  
  TutOut("Send another message with GMD to both processes.\n");
  TutOut("Receiver will cause a T_ERR_GMD_RECEIVER_TIMEOUT GMD failure.\n");
  if (!TipcSrvMsgWrite(EXAMPLE_SUBJECT, mt, TRUE,
		       T_IPC_PROP_DELIVERY_MODE, T_IPC_DELIVERY_ALL,
		       T_IPC_FT_STR, "echo Got GMD message #2.",
		       NULL)
      || !TipcSrvFlush()) {
    TutOut("Could not send CONTROL message.\n");
  }
  
  TutOut("Read data to allow acks/nacks time to come in.\n");
  if (!TipcSrvMainLoop(10.0)) {
    TutOut("Could not read from RTserver.\n");
  }

  TutOut("Send another message with GMD to a subject no one is receiving.\n");
  TutOut("RTserver will cause a T_ERR_GMD_NO_RECEIVERS GMD failure.\n");
  if (!TipcSrvMsgWrite("nobody", mt, TRUE,
		       T_IPC_PROP_DELIVERY_MODE, T_IPC_DELIVERY_ALL,
		       T_IPC_FT_STR, "echo Got GMD message #3.",
		       NULL)) {
    TutOut("Could not send CONTROL message.\n");
  }

  TutOut("Read data to allow acks/nacks time to come in.\n");
  if (!TipcSrvMainLoop(10.0)) {
    TutOut("Could not read from RTserver.\n");
  }

  TutOut("Stopping RTserver and all RTclients.\n");
  (void)TipcSrvStop(T_IPC_SRV_STOP_CLIENTS);

  return T_EXIT_SUCCESS; /* all done */
} /* main */
