/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson3/send2.cxx#1 $
 */

// send2.cxx - send a NUMERIC_DATA message with 3 variables set 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson3/send2.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt(T_MT_NUMERIC_DATA);

  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to RTserver
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) { 
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Create a message of type NUMERIC_DATA 
  msg.Create(mt);

  // Set the destination subject of the message 
  msg.Dest((T_STR)"/tutorial/lesson3");

  // Build the message with one string field: "Hello World!" 
  msg << (T_STR)"X";
  msg << 10.0;  
  msg << (T_STR)"Y";
  msg << 20.0;  
  msg << (T_STR)"Z";
  msg << 30.0;

  // Send the message 
  rtServer.Send(msg, TRUE);
  rtServer.Flush();

  // Destroy the message 
  msg.Destroy();

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
