/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson5/receive.cxx#1 $
 */

// receive.cxx - print out contents of INFO messages
// $RTHOME/examples/smrtsock/cxxtutrl/lesson5/receive.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// ================================================================== 
//..ProcessInfo --- callback for processing INFO messages 
static void T_ENTRY ProcessInfo( T_IPC_CONN conn,
			 T_IPC_CONN_PROCESS_CB_DATA data,
			 T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  T_STR MsgText;

  msg.Current(0);
  msg >> MsgText;
  cout << "Text from INFO message = " << MsgText << endl;

 } // ProcessInfo 

// ================================================================== 
//..DefaultReceiveFunc -- default callback 
static void T_ENTRY DefaultReceiveFunc( T_IPC_CONN conn,
			        T_IPC_CONN_PROCESS_CB_DATA data,
			        T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  TipcMt mt;
  T_STR  name;

  // print out the name of the type of the message 
  mt = msg.Type();
  name = mt.Name();
  cout << "Receive: unexpected message type name is <" << name << ">" << endl;
} // DefaultReceiveFunc 


// ====================================================================-- 

int main(int argc, char **argv)
{
  TipcMt mt;
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();

  // Set the project name 
  TutCommandParseStr((T_STR)"setopt project smartsockets");

  // Connect to the RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Setup callback to catch INFO messages 
  mt.Lookup(T_MT_INFO);
  rtServer.ProcessCbCreate(mt, ProcessInfo, NULL);

  // Setup default callback to process anything but INFO messages 
  rtServer.DefaultCbCreate(DefaultReceiveFunc, NULL);

  // Start receiving the "/tutorial/lesson5" subject 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson5", TRUE);


  // Read and Process all incoming messages 
  rtServer.MainLoop(T_TIMEOUT_FOREVER);

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
