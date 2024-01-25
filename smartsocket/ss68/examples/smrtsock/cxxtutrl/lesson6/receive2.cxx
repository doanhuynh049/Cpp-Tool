/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson6/receive2.cxx#1 $
 */

// receive2.cxx - print out contents of MSG_COUNT messages
// $RTHOME/examples/smrtsock/cxxtutrl/lesson6/receive2.cxx 

#define MSG_COUNT 2000
#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// ============================================================= 
//..ProcessMsgCount - callback for processing MSG_COUNT messages 

void T_ENTRY ProcessMsgCount( T_IPC_CONN conn,
		      T_IPC_CONN_PROCESS_CB_DATA data,
		      T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  T_INT4 msg_count;

  msg.Current(0);
  msg >> msg_count;
  cout << "Message Count = " << msg_count << endl;

 } // ProcessMsgCount 

// ==================================================================== 

int main(int argc, char **argv)
{
  TipcMt mt;
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();

  // Execute commands in file 
  TutCommandParseFile((T_STR)"receive2.cm");

  // Define a new message type, MSG_COUNT 
  mt.Create((T_STR)"msg_count", MSG_COUNT, (T_STR)"int4");

  // Connect to the RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Setup callback to catch MSG_COUNT messages 
  mt.Lookup(MSG_COUNT);
  rtServer.ProcessCbCreate(mt, ProcessMsgCount, NULL);

  // Start receiving the "/tutorial/lesson6" subject 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson6", TRUE);

  // Read and process all incoming messages 
  while (1) {
  rtServer.MainLoop(T_TIMEOUT_FOREVER);
  }

  return T_EXIT_SUCCESS;
} // main 
