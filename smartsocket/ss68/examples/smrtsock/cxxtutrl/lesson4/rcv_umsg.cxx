/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson4/rcv_umsg.cxx#1 $
 */

// receive_usr_msg.cxx - print out contents of messages of user-defined
//                       type XYZ_COORD_DATA 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson4/receive_usr_msg.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>
#define XYZ_COORD_DATA 1001

// ================================================================== 
//..ProcessXYZ - callback for processing XYZ_COORD_DATA messages  

static void T_ENTRY ProcessXYZ( T_IPC_CONN conn, 
			T_IPC_CONN_PROCESS_CB_DATA data, 
			T_CB_ARG arg )

{
  TipcMsg msg = data->msg;
  T_INT4 field_value;

  cout << "Received XYZ_COORD_DATA message" << endl;

   // Set current field to first field in message 
  msg.Current(0);

  // Traverse Message; print value from each field 
  msg >> field_value;
  cout << "Field 1 Value = " << field_value << endl;
  msg >> field_value;
  cout << "Field 2 Value = " << field_value << endl;
  msg >> field_value;
  cout << "Field 3 Value = " << field_value << endl;

 } // ProcessXYZ 

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


// ==================================================================== 

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt;

  // Set the project name 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to the RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Define a new message type 
  mt.Create((T_STR)"xyz_coord_data", XYZ_COORD_DATA, (T_STR)"int4 int4 int4");

  // Assign it the DATA category for logging 
  TipcSrv::LogAddMt(T_IPC_SRV_LOG_DATA, mt);

  // Setup callback to process new message type 
  rtServer.ProcessCbCreate(mt, ProcessXYZ, NULL);

  // Setup default callback to process anything but XYZ_COORD_DATA messages 
  rtServer.DefaultCbCreate(DefaultReceiveFunc, NULL);

  // Start receiving the "/tutorial/lesson4" subject 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson4", TRUE);

  // Read and process all incoming messages 
  rtServer.MainLoop(T_TIMEOUT_FOREVER);

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
