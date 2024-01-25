/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson5/polltraf.cxx#1 $
 */

// polltraf.cxx - Output message traffic info every 10 seconds
// $RTHOME/examples/smrtsock/cxxtutrl/lesson5/polltraf.cxx

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// =====================================================================

static void T_ENTRY DefaultReceiveFunc( T_IPC_CONN conn,
			        T_IPC_CONN_PROCESS_CB_DATA data,
			        T_CB_ARG arg )

{
  TipcMsg msg = data->msg;

  msg.PrintError();
} // DefaultReceiveFunc

// =====================================================================
// ProcessTrafficPoll - callback to process a
//                      MON_CLIENT_MSG_TRAFFIC_POLL_RESULT message

static void T_ENTRY ProcessTrafficPoll( T_IPC_CONN conn,
			     	T_IPC_CONN_PROCESS_CB_DATA data,
			     	T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  T_STR client_name;
  T_INT4 total_msg_recv_4;
  T_INT4 total_msg_send_4;
  T_INT4 total_byte_recv_4;
  T_INT4 total_byte_send_4;
  T_INT8 total_msg_recv_8;
  T_INT8 total_msg_send_8;
  T_INT8 total_byte_recv_8;
  T_INT8 total_byte_send_8;
  T_BOOL have_int8;

  // Set current field
  msg.Current(0);

  // Get the fields from the message
  msg >> client_name >> total_msg_recv_4 >> total_msg_send_4;
  msg >> total_byte_recv_4 >> total_byte_send_4;

  msg >> total_msg_recv_8 >> total_msg_send_8;
  msg >> total_byte_recv_8 >> total_byte_send_8;

  // Print out the new information received
  cout << "Summary of message traffic for client <" << client_name << ">" << endl;
  // If we had an error, we probably polled an old client that didn't send T_INT8's
  if (!msg) {
    cout << "   (Received 32-bit data)" << endl;
    cout << "   Total messages received = " << total_msg_recv_4 << endl;
    cout << "   Total messages sent     = " << total_msg_send_4 << endl;
    cout << "   Total bytes received    = " << total_byte_recv_4 << endl;
    cout << "   Total bytes sent        = " << total_byte_recv_4 << endl;
  }
  else {
    cout << "   (Received 64-bit data)" << endl;
    cout << "   Total messages received = " << total_msg_recv_8 << endl;
    cout << "   Total messages sent     = " << total_msg_send_8 << endl;
    cout << "   Total bytes received    = " << total_byte_recv_8 << endl;
    cout << "   Total bytes sent        = " << total_byte_recv_8 << endl;
  }
  cout << "======================================================" << endl;

} // ProcessTrafficPoll

// =====================================================================

int main(int argc, char **argv)
{
  TipcMt mt(T_MT_MON_CLIENT_MSG_TRAFFIC_POLL_RESULT);
  TipcSrv& rtServer = TipcSrv::Instance();

  // Set the project name and the unique_subject of the process
  TutCommandParseStr((T_STR)"setopt project smartsockets");
  TutCommandParseStr((T_STR)"setopt unique_subject traffic_poller");

  // Connect to the RTserver
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver." << endl;
    TutExit(T_EXIT_FAILURE);
  } // if

  // Create callback to process CLIENT_MSG_TRAFFIC poll results
  rtServer.ProcessCbCreate(mt, ProcessTrafficPoll, NULL);

  // Setup callback to receive unwanted message types
  rtServer.DefaultCbCreate(DefaultReceiveFunc, NULL);

  TipcMonClient client_mon((T_STR)T_IPC_MON_ALL);

  // Read and process all poll results every 10 seconds
  while (1) {
    cout << "==> Polling all client for message traffic info" << endl;
    client_mon.MsgTrafficPoll();
    rtServer.MainLoop(10.0);
  } // while

  return T_EXIT_SUCCESS;
} // main
