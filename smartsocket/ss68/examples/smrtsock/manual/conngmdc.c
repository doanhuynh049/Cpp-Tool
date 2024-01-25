/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/conngmdc.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* conngmdc.c -- connections GMD example client */

/*
 * The client process connects to the server process and sends two
 * messages with GMD to the server.
 */

#include "conngmd.h"

/* ================================================================ */
/*..connect_to_server -- create connection to server process */
T_IPC_CONN connect_to_server()
{
  T_IPC_CONN conn;		/* connection to server */

  conn = TipcConnCreateClient(GMD_CONN_NAME);
  if (conn == NULL) {
    TutOut("Could not create connection to server: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  create_conn_cb(conn);		/* to show inner workings of GMD */

  return conn;
} /* connect_to_server */

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_IPC_CONN conn;		/* connection to server */
  T_IPC_MT mt;			/* message type for messages */
  T_IPC_MSG msg;		/* message to send */
  T_INT4 num_pending;		/* number of messages still pending */

  TutOut("Configuring client to use file-based GMD.\n");
  TutCommandParseStr("setopt unique_subject conngmdc");

  TutOut("Creating connection to server process.\n");
  conn = connect_to_server();

  if (!TipcConnSetTimeout(conn, T_IPC_TIMEOUT_DELIVERY, 1.5)) {
    TutOut("Could not set conn delivery timeout: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Delete old GMD files.\n");
  if (!TipcConnGmdFileDelete(conn)) {
    TutOut("Could not delete old GMD files: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Create GMD area.\n");
  if (!TipcConnGmdFileCreate(conn)) {
    TutOut("Could not create GMD area: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Constructing and sending an INFO message.\n");
  mt = TipcMtLookupByNum(T_MT_INFO);
  if (mt == NULL) {
    TutOut("Could not look up INFO message type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  msg = TipcMsgCreate(mt);
  if (msg == NULL) {
    TutOut("Could not create INFO message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcMsgSetDeliveryMode(msg, T_IPC_DELIVERY_ALL)) {
    TutOut("Could not set message delivery mode: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcMsgAppendStr(msg, "GMD test #1")) {
    TutOut("Could not append field to INFO message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcConnMsgSend(conn, msg)) {
    TutOut("Could not send INFO message: error <%s>.\n",
	   TutErrStrGet());
  }

  if (!TipcMsgDestroy(msg)) {
    TutOut("Could not destroy message: error <%s>.\n",
	   TutErrStrGet());
  }

  /* use convenience functions this time */
  TutOut("Send another INFO message with GMD.\n");
  if (!TipcConnMsgWrite(conn, mt,
			T_IPC_PROP_DELIVERY_MODE, T_IPC_DELIVERY_ALL,
			T_IPC_FT_STR, "GMD test #2",
			NULL)) {
    TutOut("Could not send 2nd INFO message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Read data until all acknowledgments come in.\n");
  do {
    if (!TipcConnMainLoop(conn, 1.0)) {
      TutOut("Could not read from conn: error <%s>.\n",
	     TutErrStrGet());
      break;
    }
    if (!TipcConnGetGmdNumPending(conn, &num_pending)) {
      TutOut("Could not get pending message count: error <%s>.\n",
	     TutErrStrGet());
      break;
    }
  } while (num_pending > 0);

  if (!TipcConnDestroy(conn)) {
    TutOut("Could not destroy connection: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Client process exiting successfully.\n");
  return T_EXIT_SUCCESS;	/* all done */
} /* main */
