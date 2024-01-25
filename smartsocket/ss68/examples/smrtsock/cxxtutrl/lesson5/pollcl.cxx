/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson5/pollcl.cxx#1 $
 */

// pollcl.cxx - poll clients for information 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson5/pollcl.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// ============================================================ 
// OutputSubjectSubscribePoll - Output message of type MON_SUBJECT_POLL_RESULT 
static void OutputSubjectSubscribePoll( TipcMsg msg )
{
  T_STR subject_name;
  T_STR *recv_client_names;
  T_INT4 num_recv_clients;
  T_INT4 n;  

  // Set current field 
  msg.Current(0);

  // Get the fields from the message 
  msg.Next(&subject_name);
  msg.Next(&recv_client_names, &num_recv_clients);

  // Print out the new information received 
  cout << "Result of SUBJECT_SUBSCRIBE poll" << endl;
  cout << "Number of clients subscribed to " << subject_name
       << " = " << num_recv_clients << endl;

  for (n = 0; n < num_recv_clients; n++) {
    cout << "   [" << n << "] " << recv_client_names[n] << endl;
  } // for 

  cout << "======================================================" << endl;
  
} // OutputSubjectSubscribePoll 

// ============================================================ 

int main(int argc, char **argv)
{
  TipcMt mt(T_MT_MON_SUBJECT_SUBSCRIBE_POLL_RESULT);
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();

  // Set the project name 
  TutCommandParseStr((T_STR)"setopt project smartsockets");

  // Connect to the RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver." << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Poll for names of client processes subscribed to "/tutorial/lesson5" subject 
  TipcMon mon;

  mon.SubjectSubscribePoll((T_STR)"/tutorial/lesson5");

  // The poll will return a message of type 
  // T_MT_MON_SUBJECT_SUBSCRIBE_POLL_RESULT 
  // Wait for up to 10 seconds for the poll result 
  // We use TipcSrvMsgSearchType to search the incoming message queue
  // for the poll result 
  msg = rtServer.SearchType(10.0, mt);

  if (msg == NULL) {
    // If no message received, there are no clients subscribed to lesson5 
    cout << "Result of SUBJECT_SUBSCRIBE poll" << endl;
    cout << "Number of clients subscribed to <lesson5> = 0" << endl;
  }
  else {
    // Poll was successful, output the results 
    OutputSubjectSubscribePoll(msg);
  }

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
