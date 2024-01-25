/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson5/watchsub.cxx#1 $
 */

// watchsub.cxx - Output info whenever there is a change in subject count 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson5/watchsub.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// ======================================================================== 
// DefaultReceiveFunc - callback to process anything but MON_SUBJECT_SUBSCRIBE_STATUS 

static void T_ENTRY DefaultReceiveFunc( T_IPC_CONN conn,
			        T_IPC_CONN_PROCESS_CB_DATA data,
			        T_CB_ARG arg )

{
  TipcMsg msg = data->msg;

  msg.PrintError();
} // DefaultReceiveFunc 

// =================================================================== 
// ProcessSubjectStatus - callback to process a MON_SUBJECT_SUBSCRIBE_STATUS message 
static void T_ENTRY ProcessSubjectStatus( T_IPC_CONN conn,
			     T_IPC_CONN_PROCESS_CB_DATA data,
			     T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  T_STR subject_name;
  T_STR *recv_client_names;
  T_INT4 num_recv_clients;
  T_STR start_client;
  T_STR stop_client;
  T_INT4 n;  

  // Set current field 
  msg.Current(0);

  // Get the fields from the message 
  msg.Next(&subject_name);
  msg.Next(&recv_client_names, &num_recv_clients);
  msg.Next(&start_client);
  msg.Next(&stop_client);

  // Print out the new information received 
  cout << "Received change notice for subject <" << subject_name << ">" << endl;
  cout << "Number of clients subscribed to <" << subject_name << "> = "
       << num_recv_clients << endl;
  for (n = 0; n < num_recv_clients; n++) {
    cout << "   [" << n << "] " << recv_client_names[n] << endl;
  } // for 

  cout << "Client who just subscribed:   " << start_client << endl;
  cout << "Client who just unsubscribed: " << stop_client << endl;
  cout << "======================================================" << endl;
  
} // ProcessSubjectStatus 

// ====================================================================== 

int main(int argc, char **argv)
{
  TipcMt mt(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  TipcMsg msg;  
  TipcSrv& rtServer = TipcSrv::Instance();

  // Set the project name 
  TutCommandParseStr((T_STR)"setopt project smartsockets");

  // Connect to the RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver." << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Create callback to process subject status messages 
  // mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  rtServer.ProcessCbCreate(mt, ProcessSubjectStatus, NULL);

  // Setup callback to receive unwanted message types 
  rtServer.DefaultCbCreate(DefaultReceiveFunc, NULL);

  TipcMon mon;

  // Start "watching" the subject 
  mon.SubjectSubscribeWatch((T_STR)"/tutorial/lesson5", TRUE);

  // Read messages from RTserver and call the registered callbacks as needed 
  rtServer.MainLoop(T_TIMEOUT_FOREVER);

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
