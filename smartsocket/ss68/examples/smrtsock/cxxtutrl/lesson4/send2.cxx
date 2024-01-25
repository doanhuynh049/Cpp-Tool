/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson4/send2.cxx#1 $
 */

// send2.cxx - send a INFO and then NUMERIC_DATA message 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson4/send2.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt;

  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Send a INFO message 
  mt.Lookup(T_MT_INFO);
  rtServer.SrvWrite((T_STR)"/tutorial/lesson4", mt, TRUE,
		  T_IPC_FT_STR, "Hello World!", NULL);

  // Create a message of type NUMERIC_DATA 
  mt.Lookup(T_MT_NUMERIC_DATA);
  msg.Create(mt);

  // Set the destination subject of the message 
  msg.Dest((T_STR)"/tutorial/lesson4");

  // Build the message with 3 variable-value pairs 
  msg << (T_STR)"X" << 10.0;
  msg << (T_STR)"Y" << 20.0;
  msg << (T_STR)"Z" << 30.0;

  // Send the message 
  rtServer.Send(msg, TRUE);
  rtServer.Flush();

  // Destroy the message 
  msg.Destroy();

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
