/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/connserv.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* connserv.c -- connections example server */

/*
 * This server process waits for a client to connect to it, creates some
 * callbacks, and then loops receiving and processing messages.
 */

#include <rtworks/ipc.h>

/* ================================================================ */
/*..cb_process_numeric_data -- process callback for NUMERIC_DATA */
void T_ENTRY cb_process_numeric_data
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
/*..cb_read -- read callback */
static void T_ENTRY cb_read
(
 T_IPC_CONN conn,
 T_IPC_CONN_READ_CB_DATA data,
 T_CB_ARG arg /* really (T_IPC_MSG_FILE) */
)
{
  TutOut("Entering cb_read.\n");

  /* print out the message to the message file */
  if (!TipcMsgFileWrite((T_IPC_MSG_FILE)arg, data->msg)) {
    TutOut("Could not write to message file: error <%s>.\n",
	   TutErrStrGet());
  }
} /* cb_read */

/* ================================================================ */
/*..cb_queue -- queue callback */
static void T_ENTRY cb_queue
(
 T_IPC_CONN conn,
 T_IPC_CONN_QUEUE_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MT mt;
  T_STR name;

  TutOut("Entering cb_queue.\n");

  /* get the name of the type of the message */
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

  /* print out the position of the message being inserted/deleted */
  TutOut("A message of type %s is being %s at position %d.\n",
	 name, data->insert_flag ? "inserted" : "deleted",
	 data->position);
} /* cb_queue */

/* ================================================================ */
/*..cb_error -- error callback */
static void T_ENTRY cb_error
(
 T_IPC_CONN conn,
 T_IPC_CONN_ERROR_CB_DATA data,
 T_CB_ARG arg
)
{
  TutOut("Entering cb_error.\n");
  TutOut("The error number is %d.\n", data->err_num);
} /* cb_error */

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_IPC_CONN server_conn; /* used to accept client */
  T_IPC_CONN client_conn; /* connection to client */
  T_IPC_MT mt; /* message type for creating callbacks */
  T_IPC_MSG_FILE msg_file; /* message file for printing messages */
  T_IPC_MSG msg; /* message received and processed */

  TutOut("Creating server connection to accept clients on.\n");
  server_conn = TipcConnCreateServer("tcp:_node:5252");
  if (server_conn == NULL) {
    TutOut("Could not create server connection: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* accept one client */
  TutOut("Waiting for client to connect.\n");
  client_conn = TipcConnAccept(server_conn);
  if (client_conn == NULL) {
    TutOut("Could not accept client: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* destroy server conn: it's not needed anymore */
  TutOut("Destroying server connection.\n");
  if (!TipcConnDestroy(server_conn)) {
    TutOut("Could not destroy server connection: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* create callbacks to be executed when certain operations occur */
  TutOut("Create callbacks.\n");

  /* process callback */
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (mt == NULL) {
    TutOut("Could not look up NUMERIC_DATA msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (TipcConnProcessCbCreate(client_conn, mt, 
			      cb_process_numeric_data,
			      NULL) == NULL) {
    TutOut("Could not create NUMERIC_DATA process cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* default callback */
  if (TipcConnDefaultCbCreate(client_conn, 
			      cb_default, NULL) == NULL) {
    TutOut("Could not create default cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* create a message file to use in read callback */
  msg_file = TipcMsgFileCreateFromFile(stdout,
				       T_IPC_MSG_FILE_CREATE_WRITE);
  if (msg_file == NULL) {
    TutOut("Could not create message file from stdout: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* read callback */
  if (TipcConnReadCbCreate(client_conn,
			   NULL, /* global callback */
			   cb_read,
			   msg_file) == NULL) {
    TutOut("Could not create read cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* queue callback */
  if (TipcConnQueueCbCreate(client_conn,
			    NULL, /* global callback */
			    cb_queue,
			    NULL) == NULL) {
    TutOut("Could not create queue cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* error callback */
  if (TipcConnErrorCbCreate(client_conn,
			    cb_error,
			    NULL) == NULL) {
    TutOut("Could not create error cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Read and process all messages.\n");
  while ((msg = TipcConnMsgNext(client_conn, T_TIMEOUT_FOREVER))
	 != NULL) {
    if (!TipcConnMsgProcess(client_conn, msg)) {
      TutOut("Could not process message: error <%s>.\n",
	     TutErrStrGet());
    }
    if (!TipcMsgDestroy(msg)) {
      TutOut("Could not destroy message: error <%s>.\n",
	     TutErrStrGet());
    }
  }
  
  /* make sure we reached the end of the data */
  if (TutErrNumGet() != T_ERR_EOF) {
    TutOut("Did not reach end of data: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcConnDestroy(client_conn)) {
    TutOut("Could not destroy client connection: error <%s>.\n",
	   TutErrStrGet());
  }
  
  if (!TipcMsgFileDestroy(msg_file)) {
    TutOut("Could not destroy message file: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Server process exiting successfully.\n");
  return T_EXIT_SUCCESS; /* all done */
} /* main */

