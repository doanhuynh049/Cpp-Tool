/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtclgmdr.c#1 $
 */

/* 
 * Mentioned in "RTserver and RTclient" chapter in the
 * "SmartSockets User's Guide". 
 */

/* rtclgmdr.c -- RTclient GMD example receiver */

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
  
  TutOut("Restart RTserver with proper config for this example.\n");
  (void)TutSystem("rtserver -stop_clients");
  (void)TutSystem("rtserver -command ./rtsrvgmd.cm");

  /* Configure receiver to use file-based GMD. */
  TutCommandParseStr("setopt unique_subject gmd_receiver");
  
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
  
  TutOut("Wait for message, then simulate crash and restart.\n");
  TutOut("This will cause a T_ERR_GMD_RECEIVER_EXIT GMD failure.\n");
  if (!TipcSrvRead(60.0)) {
    TutOut("Message did not arrive in time.\n");
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Could not fully destroy connection to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Wait for message, then simulate crash w/o restart.\n");
  TutOut("This will cause a T_ERR_GMD_RECEIVER_TIMEOUT GMD failure.\n");
  TutCommandParseStr("setopt server_disconnect_mode warm");
  
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
  
  if (!TipcSrvRead(15.0)) {
    TutOut("Message did not arrive in time.\n");
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Could not fully destroy connection to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  return T_EXIT_SUCCESS;
} /* main */
