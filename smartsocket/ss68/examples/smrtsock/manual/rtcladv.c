/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtcladv.c#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* rtcladv.c -- RTclient advanced example */

/*
 * This RTclient creates and destroys its connection to RTserver in 
 * various ways, and publishes messages to a subject it is subscribing 
 * to.
 */

#include <rtworks/ipc.h>
#include "rtclient.h"

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
  TutOut("Message type name is <%s>\n", name);
} /* cb_default */

/* ================================================================ */
/*..cb_server_create -- server create callback */
static void T_ENTRY cb_server_create
(
 T_IPC_CONN conn,
 T_IPC_SRV_CREATE_CB_DATA data,
 T_CB_ARG arg
)
{

  /* Create other callbacks if we did not have a warm connection. */
  if (data->old_conn_status == T_IPC_SRV_CONN_WARM) {
    TutOut("We already had a warm connection to RTserver, which ");
    TutOut("preserves all callbacks.\n");
    return;			/* nothing to do */
  }

  /* A larger process would create many callbacks here. This */
  /* simple example only creates a connection default callback. */
  TutOut("Creating other callbacks.\n");

  /* default callback */
  if (TipcSrvDefaultCbCreate(cb_default, NULL) == NULL) {
    TutOut("Could not create default callback: error <%s>.\n",
	   TutErrStrGet());
  }

} /* cb_server_create */

/* ================================================================ */
/*..cb_server_destroy -- server destroy callback */
static void T_ENTRY cb_server_destroy
(
 T_IPC_CONN conn,
 T_IPC_SRV_DESTROY_CB_DATA data,
 T_CB_ARG arg
)
{
  TutOut("Entering cb_server_destroy.\n");
  if (data->new_conn_status == T_IPC_SRV_CONN_WARM) {
    TutOut("Leaving a warm connection to RTserver.\n");
  }
  else {
    TutOut("Leaving no connection to RTserver.\n");
  }

} /* cb_server_destroy */

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

  /* create callbacks to be executed when certain operations occur */
  TutOut("Create callbacks.\n");

  /* If an RTclient will be creating and destroying its connection */
  /* to RTserver over and over, it should use a server create */
  /* callback to create the rest of its callbacks. */

  /* server create callback */
  if (TipcSrvCreateCbCreate(cb_server_create, NULL) == NULL) {
    TutOut("Could not create server create callback: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* server destroy callback */
  if (TipcSrvDestroyCbCreate(cb_server_destroy, NULL) == NULL) {
    TutOut("Could not create server destroy callback: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Creating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Destroy connection to RTserver to show how this can be done. */
  TutOut("\nDestroying connection to RTserver but leave it warm.\n");
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_WARM)) {
    TutOut("Could not destroy connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Creating connection to RTserver from warm connection.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Completely destroying connection to RTserver.\n");
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Could not destroy connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* At this point, the RTclient could proceed as if it had */
  /* never been connected to RTserver at all. For this example */
  /* we want to reconnect eventually, though. */

  /* Set the option Server_Auto_Connect to FALSE to show how */
  /* an RTclient can buffer outgoing messages even when not */
  /* connected to RTserver. */
  TutCommandParseStr("setopt server_auto_connect false");

  TutOut("\nCreating warm connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_WARM)) {
    TutOut("Could not create warm connection to RTserver.\n");
    TutOut("  error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Now proceed with normal processing even though we only */
  /* have a warm connection. */
  TutOut("Start subscribing to standard subjects.\n");
  if (!TipcSrvStdSubjectSetSubscribe(TRUE, TRUE)) {
    TutOut("Could not start subscribing to standard subjects.\n");
    TutOut("  error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Start subscribing to the %s subject.\n", EXAMPLE_SUBJECT);
  if (!TipcSrvSubjectSetSubscribe(EXAMPLE_SUBJECT, TRUE)) {
    TutOut("Could not start subscribing to %s subject: error <%s>.\n",
	   EXAMPLE_SUBJECT, TutErrStrGet());
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

  TutOut("\nCreating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create connection to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Each RTclient automatically creates a connection process */
  /* callback for CONTROL messages. Use this to send the command */
  /* "quit force" to the receiver's command interface. */
  TutOut("Publishing a CONTROL message to stop ourself.\n");
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
