/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/polltraf.c#1 $
 */

/*
  ------------------------------------------------------------------
  polltraf.c

  USAGE
    polltraf <project> <unique_subject> <poll_interval>

  This program gathers message traffic information on the RTclient
  specified in the command line arg <unique_subject> in
  project <project>. The info is polled for on a
  regular interval as specified in <poll_interval>. You can gather
  message traffic info on all clients in a project by
  specifying "/..." for <unique_subject>. <poll_interval> is
  always specified in seconds (as an integer).

    Sample usage:
      polltraf tank my_ie 10
      polltraf tank /... 30
  ------------------------------------------------------------------
*/

#include <rtworks/ipc.h>

/* ================================================================ */
/*..cb_default - default callback */
static void T_ENTRY cb_default
(
 T_IPC_CONN conn,
 T_IPC_CONN_DEFAULT_CB_DATA data,
 T_CB_ARG arg
)
{
  if (!TipcMsgPrintError(data->msg)) {
    TutOut("Could not print message: error <%s>.\n",
     TutErrStrGet());
  }
} /* cb_default */

/* =============================================================== */
/*..process_traffic_poll - callback to process a
                           MON_CLIENT_MSG_TRAFFIC_POLL_RESULT message */
static void T_ENTRY process_traffic_poll
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
  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
           TutErrStrGet());
    return;
  }

  /* Get the fields from the message */
  if (!TipcMsgRead(data->msg, T_IPC_FT_STR,  &client_name,
                              T_IPC_FT_INT4, &total_msg_recv_4,
                              T_IPC_FT_INT4, &total_msg_send_4,
                              T_IPC_FT_INT4, &total_byte_recv_4,
                              T_IPC_FT_INT4, &total_byte_send_4,
                              NULL)) {
    TutOut("Unable to access message traffic message: error <%s>.\n",
           TutErrStrGet());
    return;
  }

  TutOut("Summary of message traffic for RTclient <%s>\n", client_name);
  if (TipcMsgRead(data->msg, T_IPC_FT_INT8, &total_msg_recv_8,
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

} /* process_traffic_poll */

/* =============================================================== */
int main
(
 int argc,
 char **argv
)
{
  T_STR project_name;
  T_STR unique_subject;
  T_REAL8 poll_interval;
  T_IPC_MT mt;
  T_OPTION option;

  /* Check the command line arguments */
  if (argc != 4) {
    TutOut("Usage: polltraf <project> <unique_subject> ");
    TutOut("<poll_interval>\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Save pointers to command line arguments */
  project_name = argv[1];
  unique_subject = argv[2];
  poll_interval = atof(argv[3]);

  /* Check that the polling interval is greater than zero */
  if (poll_interval <= 0.0) {
    TutOut("Poll_Interval must be greater than zero.\n");
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Polling RTclient <%s> in project <%s> ",
   unique_subject, project_name);
  TutOut("every <%g> seconds...\n", poll_interval);

  /* Set the project name */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not look up option <project>: error <%s>.\n",
     TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TutOptionSetEnum(option, project_name)) {
    TutOut("Could not set option <project>: error <%s>.\n",
     TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver: error <%s>.\n",
     TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Create callback to process CLIENT_MSG_TRAFFIC poll results */
  mt = TipcMtLookupByNum(T_MT_MON_CLIENT_MSG_TRAFFIC_POLL_RESULT);
  if (mt == NULL) {
    TutOut("Could not look up message type ");
    TutOut("MON_CLIENT_MSG_TRAFFIC_POLL_RESULT: error <%s>.\n",
     TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (TipcSrvProcessCbCreate(mt, process_traffic_poll, NULL) == NULL){
    TutOut("Could not set up callback to process ");
    TutOut("MON_CLIENT_MSG_POLL_RESULT: error <%s>.\n",
     TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Create default callback to receive unwanted message types */
  if (TipcSrvDefaultCbCreate(cb_default, NULL) == NULL) {
    TutOut("Could not create default callback: error <%s>.\n",
     TutErrStrGet());
  }

  /* Read and process poll results every "poll_interval" seconds */
  while (1) {
    TutOut("==> Polling RTclient <%s> for message traffic info...\n",
           unique_subject);

    /* Initiate poll for message traffic info about an RTclient */
    if (!TipcMonClientMsgTrafficPoll(unique_subject)) {
      TutOut("Unable to poll for message traffic for RTclient <%s>.\n",
             unique_subject);
      TutOut(" error <%s>.\n", TutErrStrGet());
    }

    /* Process the poll results which arrive */
    if (!TipcSrvMainLoop(poll_interval)) {
      TutOut("TipcSrvMainLoop failed: error <%s>.\n",
       TutErrStrGet());
    }
  }

  /* This line should not be reached. */
  return T_EXIT_FAILURE;
} /* main */
