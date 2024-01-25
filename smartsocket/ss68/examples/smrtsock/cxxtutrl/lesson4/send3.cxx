/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson4/send3.cxx#1 $
 */

// send3.cxx - send a INFO and then a series of NUMERIC_DATA messages 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson4/send3.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  T_INT4 i;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt_numeric(T_MT_NUMERIC_DATA);
  TipcMt mt_info(T_MT_INFO);

  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Send a INFO message 
  rtServer.SrvWrite((T_STR)"/tutorial/lesson4", mt_info, TRUE,
		  T_IPC_FT_STR, "Hello World!", NULL);

  // Create a message of type NUMERIC_DATA 
  msg.Create(mt_numeric);

  // Set the destination subject of the message 
  msg.Dest((T_STR)"/tutorial/lesson4");

  for (i = 0; i < 30; i = i + 3) {
    // In order to re-use the message, reset the number of the 
    // fields in the message to zero 
    msg.NumFields(0);

    // Build the message with 3 variable-value pairs 
    msg.Append((T_STR)"X", (T_REAL8)i);
    msg.Append((T_STR)"Y", (T_REAL8)i+1);
    msg.Append((T_STR)"Z", (T_REAL8)i+2);

    // Send the message 
  rtServer.Send(msg, TRUE);
  rtServer.Flush();
  } // for 

  // Destroy the message 
  msg.Destroy();

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
