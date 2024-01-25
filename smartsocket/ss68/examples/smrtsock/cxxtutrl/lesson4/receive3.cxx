/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson4/receive3.cxx#1 $
 */

// receive3.cxx - print out contents of NUMERIC_DATA messages via callback 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson4/receive3.cxx

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// ================================================================== 
//..ProcessNumData - callback for processing NUMERIC_DATA messages 
static void T_ENTRY ProcessNumData( T_IPC_CONN conn,
			    T_IPC_CONN_PROCESS_CB_DATA data,
			    T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  T_STR var_name;
  T_REAL8 var_value;

  cout << "Received NUMERIC_DATA message" << endl;

   // Set current field to first field in message 
  msg.Current(0);

  // Traverse Message; print each name-value pair out 
  while (msg.Next(&var_name, &var_value)) {
    cout << "Var Name = " << var_name 
	 << "; Value = " << TutRealToStr(var_value) << endl;
  } // while 

 } // ProcessNumData 

// ================================================================== 
//..DefaultReceiveFunc - default callback 
static void T_ENTRY DefaultReceiveFunc( T_IPC_CONN conn,
			        T_IPC_CONN_PROCESS_CB_DATA data,
			        T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  TipcMt mt;
  T_STR name;

  // print out the name of the type of the message 
  mt = msg.Type();
  name = mt.Name();
  cout << "Receive: unexpected message type name is <" << name << ">" << endl;
} // DefaultReceiveFunc 

// ====================================================================-- 

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMt mt(T_MT_NUMERIC_DATA);
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();

  // Set the project name 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to the RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Setup callback to catch NUMERIC_DATA messages 
  rtServer.ProcessCbCreate(mt, ProcessNumData, NULL);

  // Setup default callback to process anything but NUMERIC_DATA messages 
  rtServer.DefaultCbCreate(DefaultReceiveFunc, NULL);

  // Start receiving the "/tutorial/lesson4" subject 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson4", TRUE);

  // Read and Process all incoming messages 
  rtServer.MainLoop(T_TIMEOUT_FOREVER);

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
