/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/conngmds.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* conngmds.c -- connections GMD example server */

/*
 * This server process waits for a client to connect to it, creates some 
 * callbacks to show how GMD works, and then loops receiving and 
 * processing messages.
 */

#include "conngmd.h"

/* ================================================================ */
/*..accept_client -- accept connection from new client */
T_IPC_CONN accept_client
(
 T_IPC_CONN server_conn
)
{
  T_IPC_CONN client_conn;	/* connection to client */

  client_conn = TipcConnAccept(server_conn);
  if (client_conn == NULL) {
    TutOut("Could not accept client: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  create_conn_cb(client_conn);	/* to show inner workings of GMD */

  return client_conn;
} /* accept_client */

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_IPC_CONN server_conn;	/* used to accept client */
  T_IPC_CONN client_conn;	/* connection to client */

  TutOut("Configuring server to use file-based GMD.\n");
  TutCommandParseStr("setopt unique_subject conngmds");

  TutOut("Creating server connection to accept clients on.\n");
  server_conn = TipcConnCreateServer(GMD_CONN_NAME);
  if (server_conn == NULL) {
    TutOut("Could not create server connection: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* accept one client */
  TutOut("Waiting for client to connect.\n");
  client_conn = accept_client(server_conn);

  /* destroy server conn: it's not needed anymore */
  TutOut("Destroying server connection.\n");
  if (!TipcConnDestroy(server_conn)) {
    TutOut("Could not destroy server connection: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Delete old GMD files.\n");
  if (!TipcConnGmdFileDelete(client_conn)) {
    TutOut("Could not delete old GMD files: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Read and process messages.\n");
  if (!TipcConnMainLoop(client_conn, T_TIMEOUT_FOREVER)
      && TutErrNumGet() != T_ERR_EOF) {
    TutOut("Could not read and process messages: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcConnDestroy(client_conn)) {
    TutOut("Could not destroy client connection: error <%s>.\n",
	   TutErrStrGet());
  }

  TutOut("Server process exiting successfully.\n");
  return T_EXIT_SUCCESS;	/* all done */
} /* main */







