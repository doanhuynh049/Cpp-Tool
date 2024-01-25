/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson5/polltraf.c#1 $
 */

/* polltraf.c - Output message traffic info every 10 seconds */
/* $RTHOME/examples/smrtsock/tutorial/lesson5/polltraf.c */

#include <rtworks/ipc.h>

static void T_ENTRY DefaultReceiveFunc(T_IPC_CONN conn,
               T_IPC_CONN_PROCESS_CB_DATA data,
               T_CB_ARG arg);

static void T_ENTRY ProcessTrafficPoll(T_IPC_CONN conn,
               T_IPC_CONN_PROCESS_CB_DATA data,
               T_CB_ARG arg);

/* ========================================================================= */
/*..DefaultReceiveFunc -- default callback */
static void T_ENTRY DefaultReceiveFunc
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  TipcMsgPrintError(data->msg);
} /* DefaultReceiveFunc */

/* ========================================================================= */
/* ProcessTrafficPoll - callback to process a                                */
/*                                MON_CLIENT_MSG_TRAFFIC_POLL_RESULT message */
static void T_ENTRY ProcessTrafficPoll
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_STR client_name;
  T_INT4 total_msg_recv_4;
  T_INT4 total_msg_send_4;
  T_INT4 total_byte_recv_4;
  T_INT4 total_byte_send_4;
  T_INT8 total_msg_recv_8;
  T_INT8 total_msg_send_8;
  T_INT8 total_byte_recv_8;
  T_INT8 total_byte_send_8;

  /* Set current field */
  TipcMsgSetCurrent(msg, 0);

  /* Get the fields from the message */
  TipcMsgRead(data->msg, T_IPC_FT_STR,  &client_name,
                         T_IPC_FT_INT4, &total_msg_recv_4,
                         T_IPC_FT_INT4, &total_msg_send_4,
                         T_IPC_FT_INT4, &total_byte_recv_4,
                         T_IPC_FT_INT4, &total_byte_send_4,
                         T_NULL);

  TutOut("Summary of message traffic for client <%s>\n", client_name);
  if (TipcMsgRead(data->msg,
      T_IPC_FT_INT8, &total_msg_recv_8,
      T_IPC_FT_INT8, &total_msg_send_8,
      T_IPC_FT_INT8, &total_byte_recv_8,
      T_IPC_FT_INT8, &total_byte_send_8,
      NULL)) {
    /* Print out the new information received */
    TutOut("   (Received 64-bit data)\n");
    TutOut("   Total messages received = " T_INT8_SPEC "\n", total_msg_recv_8);
    TutOut("   Total messages sent     = " T_INT8_SPEC "\n", total_msg_send_8);
    TutOut("   Total bytes received    = " T_INT8_SPEC "\n", total_byte_recv_8);
    TutOut("   Total bytes sent        = " T_INT8_SPEC "\n", total_byte_recv_8);
  }
  else {
    /* Print out the new information received */
    TutOut("   (Received 32-bit data)\n");
    TutOut("   Total messages received = " T_INT4_SPEC "\n", total_msg_recv_4);
    TutOut("   Total messages sent     = " T_INT4_SPEC "\n", total_msg_send_4);
    TutOut("   Total bytes received    = " T_INT4_SPEC "\n", total_byte_recv_4);
    TutOut("   Total bytes sent        = " T_INT4_SPEC "\n", total_byte_recv_4);
  }
  TutOut("======================================================\n");

} /* ProcessTrafficPoll */

/* ========================================================================= */
int main(int argc, char **argv)
{
  T_IPC_MT mt;

  /* Set the project name and the unique_subject of the process */
  TutCommandParseStr("setopt project smartsockets");
  TutCommandParseStr("setopt unique_subject /traffic/poller");
  TutCommandParseStr("setopt monitor_scope /...");

  /* Connect to the RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Create callback to process MON_CLIENT_MSG_TRAFFIC poll results */
  mt = TipcMtLookupByNum(T_MT_MON_CLIENT_MSG_TRAFFIC_POLL_RESULT);
  TipcSrvProcessCbCreate(mt, ProcessTrafficPoll, NULL);

  /* Setup callback to receive unwanted message types */
  TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);

  /* Read and process all poll results every 10 seconds */
  while (1) {
    TutOut("==> Polling all client for message traffic info\n");
    TipcMonClientMsgTrafficPoll(T_IPC_MON_ALL);
    TipcSrvMainLoop(10.0);
  } /* while */

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
