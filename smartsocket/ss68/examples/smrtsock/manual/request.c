/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/request.c#1 $
 */

/* ------------------------------------------------------------- */
/* request.c                                                     */
/*                                                               */
/* USAGE                                                         */
/*    request                                                    */
/*                                                               */
/* The following example uses both timer events and message      */
/* events. A seperate thread is created, running a single        */
/* dispatcher firing timer events.  Message events are processed */
/* inside the main thread of execution                           */
/*                                                               */
/* The main thread creates a connection to the RTserver and      */
/* processes incoming messages. Two timer events are also        */
/* added to a detached dispatcher,  each of which sends a        */
/* request message on a periodic basis. The request message      */
/* results in response messages being sent back to the client.   */
/* Two message events are created, one each for the two types of */
/* response messages. The two message events will each be        */
/* invoked in the main thread.                                   */
/*                                                               */
/* The example program response.c is provided to send the        */
/* response messages.                                            */
/* ------------------------------------------------------------- */

#include <rtworks/ipc.h>

#define T_MT_CLIENT_REQUEST   101
#define T_MT_CLIENT_RESPONSE  201

#define T_MT_SERVER_REQUEST   102
#define T_MT_SERVER_RESPONSE  202

/* ------------------------------------------------------------------*/
static void T_ENTRY clientRequestEventTimerFunc
(
 T_IPC_EVENT event,
 T_IPC_EVENT_DATA data,
 T_PTR arg
)
{
  /* ------------------------------------------------------ */
  /* This is the user-defined function invoked whenever the */
  /* associated timer event is triggered. It sends a client */
  /* request message. A message type event will process the */
  /* client response message sent in reply to this request  */
  /* message.                                               */
  /* ------------------------------------------------------ */

  TutOut("%s: CLIENT REQUEST - sending\n", TutGetWallTimeStr());

  /* -------------------------------- */
  /* Send the client request message. */
  /* -------------------------------- */
  TipcSrvMsgWrite("/request", TipcMtLookupByNum(T_MT_CLIENT_REQUEST), T_TRUE,
                  T_NULL);
  TipcSrvFlush();

} /* clientRequestEventTimerFunc */

/* ------------------------------------------------------------------*/
static void T_ENTRY serverRequestEventTimerFunc
(
 T_IPC_EVENT event,
 T_IPC_EVENT_DATA data,
 T_PTR arg
)
{
  /* ------------------------------------------------------ */
  /* This is the user-defined function invoked whenever the */
  /* associated timer event is triggered. It sends a server */
  /* request message. A message type event will process the */
  /* server response message sent in reply to this request  */
  /* message.                                               */
  /* ------------------------------------------------------ */

  TutOut("%s: SERVER REQUEST - sending\n", TutGetWallTimeStr());

  /* -------------------------------- */
  /* Send the server request message. */
  /* -------------------------------- */
  TipcSrvMsgWrite("/request", TipcMtLookupByNum(T_MT_SERVER_REQUEST), T_TRUE,
                  T_NULL);
  TipcSrvFlush();

} /* serverRequestEventTimerFunc */

/* ------------------------------------------------------------------*/
static void T_ENTRY clientResponseEventMsgTypeFunc
(
 T_IPC_EVENT event,
 T_IPC_EVENT_DATA data,
 T_PTR arg
)
{
  /* ------------------------------------------------------ */
  /* This is the user-defined function invoked whenever the */
  /* associated message event is triggered. It processes a  */
  /* client response message sent in response to a client   */
  /* request message.                                       */
  /* ------------------------------------------------------ */

  TutOut("%s: CLIENT RESPONSE - processing\n", TutGetWallTimeStr());

} /* clientResponseEventMsgTypeFunc */

/* ------------------------------------------------------------------*/
static void T_ENTRY serverResponseEventMsgTypeFunc
(
 T_IPC_EVENT event,
 T_IPC_EVENT_DATA data,
 T_PTR arg
)
{
  /* ------------------------------------------------------ */
  /* This is the user-defined function invoked whenever the */
  /* associated message event is triggered. It processes a  */
  /* server response message sent in response to a server   */
  /* request message.                                       */
  /* ------------------------------------------------------ */

  TutOut("%s: SERVER RESPONSE - processing\n", TutGetWallTimeStr());

} /* serverResponseEventMsgTypeFunc */

/* ---------------------------------------------------------------------- */
int main
(
 int argc,
 char **argv
)
{
  T_IPC_SRV srv;
  T_IPC_DISPATCHER main_disp;
  T_IPC_DISPATCHER timer_disp;

  /* -------------------------------------------- */
  /* Make the SmartSockets libraries thread-safe. */
  /* -------------------------------------------- */
  TipcInitThreads();

  /* ------------------------------------ */
  /* Parse the command file if available. */
  /* ------------------------------------ */
  TutCommandParseFile("request.cm");

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

  /* -------------------------------------- */
  /* Get the connection's T_IPC_SRV object. */ 
  /* -------------------------------------- */
  TipcSrvGetSrv(&srv);

  /* ----------------------------------------------------- */
  /* Create a dispatcher, and a detached dispatcher that   */
  /* runs in its own thread and fires timer events.        */
  /* ----------------------------------------------------- */
  main_disp = TipcDispatcherCreate();
  timer_disp = TipcDispatcherCreateDetached();

  /* ----------------------------------------------------- */
  /* Add the connection to the RTserver to the dispatcher. */
  /* ----------------------------------------------------- */
  TipcDispatcherSrvAdd(main_disp, srv);

  /* ----------------------------------------------------- */
  /* Add two timer events to the detached dispatcher,      */
  /* one each to send the two types of requests.           */
  /* Both timer events will fire in a seperate thread      */
  /* ----------------------------------------------------- */
  TipcEventCreateTimer(timer_disp, 1.0, clientRequestEventTimerFunc, T_NULL);
  TipcEventCreateTimer(timer_disp, 5.0, serverRequestEventTimerFunc, T_NULL);

  /* --------------------------------------------------------- */
  /* A main dispatcher will run handling events on this thread.*/
  /* This dispatcher will process the two types of response    */
  /* message.  A similar setup could also be used to processes */
  /* messages based upon subject.                              */              
  /*                                                           */
  /* Timer events will publish messages from a background      */
  /* thread.                                                   */
  /*                                                           */
  /* Note:  Do not share a listening server connection across  */
  /* seperate dispatchers.  This can result in frequent        */
  /* locking conflicts as multiple dispatcher loops attempt to */
  /* read simultaneously on the connection.                    */
  /* --------------------------------------------------------- */
  TipcEventCreateMsgType(main_disp, 
                         srv,
                         TipcMtLookupByNum(T_MT_CLIENT_RESPONSE),
                         clientResponseEventMsgTypeFunc,
                         T_NULL);

  TipcEventCreateMsgType(main_disp, 
                         srv,
                         TipcMtLookupByNum(T_MT_SERVER_RESPONSE),
                         serverResponseEventMsgTypeFunc,
                         T_NULL);

  /* -------------------------------------------------------------- */
  /* Dispatch the incoming messages from the RTserver.              */
  /* -------------------------------------------------------------- */
  TipcDispatcherMainLoop(main_disp, T_TIMEOUT_FOREVER);

  TutExit(T_EXIT_SUCCESS);
} /* main */

