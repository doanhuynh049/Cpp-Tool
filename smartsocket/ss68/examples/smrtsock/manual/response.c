/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/response.c#1 $
 */

/* ------------------------------------------------------------- */
/* response.c                                                    */
/*                                                               */
/* USAGE                                                         */
/*    response                                                   */
/*                                                               */
/* Responds to requests issued by the request example. See       */
/* request.c for more information.                               */
/* ------------------------------------------------------------- */

#include <rtworks/ipc.h>

#define T_MT_CLIENT_REQUEST   101
#define T_MT_CLIENT_RESPONSE  201

#define T_MT_SERVER_REQUEST   102
#define T_MT_SERVER_RESPONSE  202

/* ---------------------------------------------------------------------- */
static void T_ENTRY clientRequestCbFunc
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR sender;

  TipcMsgGetSender(data->msg, &sender);

  TutOut("%s: CLIENT REQUEST - received\n", TutGetWallTimeStr());

  /* --------------------------------------------------------------------- */
  /* Respond to the client request message with a client response message. */
  /* --------------------------------------------------------------------- */
  TipcSrvMsgWrite(sender, TipcMtLookupByNum(T_MT_CLIENT_RESPONSE), T_TRUE,
                  T_NULL);
  TipcSrvFlush();
} /* clientRequestCbFunc */

/* ---------------------------------------------------------------------- */
static void T_ENTRY serverRequestCbFunc
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR sender;

  TipcMsgGetSender(data->msg, &sender);

  TutOut("%s: SERVER REQUEST - received\n", TutGetWallTimeStr());

  /* --------------------------------------------------------------------- */
  /* Respond to the server request message with a server response message. */
  /* --------------------------------------------------------------------- */
  TipcSrvMsgWrite(sender, TipcMtLookupByNum(T_MT_SERVER_RESPONSE), T_TRUE,
                  T_NULL);
  TipcSrvFlush();
} /* serverRequestCbFunc */

/* ---------------------------------------------------------------------- */
int main
(
 int argc,
 char **argv
)
{
  /* ------------------------------------ */
  /* Parse the command file if available. */
  /* ------------------------------------ */
  TutCommandParseFile("response.cm");

  /* ------------------------- */
  /* Create the message types. */
  /* ------------------------- */
  TipcMtCreate("client request", T_MT_CLIENT_REQUEST, "verbose");
  TipcMtCreate("client response", T_MT_CLIENT_RESPONSE, "verbose");

  TipcMtCreate("server request", T_MT_SERVER_REQUEST, "verbose");
  TipcMtCreate("server response", T_MT_SERVER_RESPONSE, "verbose");

  /* ------------------------------------ */
  /* Create a connection to the RTserver. */
  /* ------------------------------------ */
  TipcSrvCreate(T_IPC_SRV_CONN_FULL);

  /* ------------------------------- */
  /* Subscribe to /request messages. */
  /* ------------------------------- */
  TipcSrvSubjectSetSubscribe("/request", T_TRUE);
  TipcSrvFlush();

  /* ------------------------------------------------------------------ */
  /* Add two callbacks, one each for the two types of request messages. */
  /* ------------------------------------------------------------------ */
  TipcSrvProcessCbCreate(TipcMtLookupByNum(T_MT_CLIENT_REQUEST),
                         clientRequestCbFunc,
                         T_NULL);
  TipcSrvProcessCbCreate(TipcMtLookupByNum(T_MT_SERVER_REQUEST),
                         serverRequestCbFunc,
                         T_NULL);

  /* ----------------------------------- */
  /* Process messages from the RTserver. */
  /* ----------------------------------- */
  TipcSrvMainLoop(T_TIMEOUT_FOREVER);

  TutExit(T_EXIT_SUCCESS);
} /* main */

