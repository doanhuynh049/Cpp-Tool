/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/connclnt.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* connclnt.c -- connections example client */

/*
 * The client process connects to the server process and sends two
 * messages to the server.
 */

#include <rtworks/ipc.h>

/* ================================================================ */
/*..cb_write -- write callback */
static void T_ENTRY cb_write
(
 T_IPC_CONN conn,
 T_IPC_CONN_WRITE_CB_DATA data,
 T_CB_ARG arg /* really (T_IPC_MSG_FILE) */
)
{
  TutOut("Entering cb_write.\n");

  /* print out the message to the message file */
  if (!TipcMsgFileWrite((T_IPC_MSG_FILE)arg, data->msg)) {
    TutOut("Could not write to message file: error <%s>.\n",
	   TutErrStrGet());
  }
} /* cb_write */

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_IPC_CONN conn; /* connection to server */
  T_IPC_MT mt; /* message type for creating callbacks and messages */
  T_IPC_MSG_FILE msg_file; /* message file for printing messages */
  T_IPC_MSG msg; /* message to send */

  TutOut("Creating connection to server process.\n");
  conn = TipcConnCreateClient("tcp:_node:5252");
  if (conn == NULL) {
    TutOut("Could not create connection to server: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* create callbacks to be executed when certain operations occur */
  TutOut("Create callbacks.\n");
  
  /* create a message file to use in write callback */
  msg_file = TipcMsgFileCreateFromFile(stdout,
				       T_IPC_MSG_FILE_CREATE_WRITE);
  if (msg_file == NULL) {
    TutOut("Could not create message file from stdout: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);			
  }
  
  /* write callback */
  if (TipcConnWriteCbCreate(conn,
			    NULL, /* global callback */
			    cb_write,
			    msg_file) == NULL) {
    TutOut("Could not create write cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Constructing and sending a NUMERIC_DATA message.\n");
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (mt == NULL) {
    TutOut("Could not look up NUMERIC_DATA msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  msg = TipcMsgCreate(mt);
  if (msg == NULL) {
    TutOut("Could not create NUMERIC_DATA message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcMsgWrite(msg,
		    T_IPC_FT_STR, "voltage",
		    T_IPC_FT_REAL8, 33.4534,
		    T_IPC_FT_STR, "switch_pos",
		    T_IPC_FT_REAL8, 0.0,
		    NULL)) {
    TutOut("Could not append to NUMERIC_DATA msg: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcConnMsgSend(conn, msg)) {
    TutOut("Could not send NUMERIC_DATA message: error <%s>.\n",
	   TutErrStrGet());
  }
  
  TutOut("Constructing and sending an INFO message.\n");
  mt = TipcMtLookupByNum(T_MT_INFO);
  if (mt == NULL) {
    TutOut("Could not look up INFO message type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* just reuse previous message */
  if (!TipcMsgSetType(msg, mt)) {
    TutOut("Could not set message type: error <%s>.\n",
	   TutErrStrGet());
  }
  if (!TipcMsgSetNumFields(msg, 0)) {
    TutOut("Could not set message num fields: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcMsgAppendStr(msg, "Now is the time")) {
    TutOut("Could not append fields to INFO message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcConnMsgSend(conn, msg)) {
    TutOut("Could not send INFO message: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcConnFlush(conn)) {
    TutOut("Could not flush buffered outgoing msgs: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcMsgDestroy(msg)) {
    TutOut("Could not destroy message: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcConnDestroy(conn)) {
    TutOut("Could not destroy connection: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcMsgFileDestroy(msg_file)) {
    TutOut("Could not destroy message file: error <%s>.\n",
	   TutErrStrGet());
  }
  
  TutOut("Client process exiting successfully.\n");
  return T_EXIT_SUCCESS; /* all done */
} /* main */
