/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson4/snd_umsg.cxx#1 $
 */

// send_umsg.cxx - create and send a series of messages of user-defined type 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson4/send_umsg.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>
#define XYZ_COORD_DATA 1001

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  T_INT4 i;
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

  // Define a new message type 
  mt.Create((T_STR)"xyz_coord_data", XYZ_COORD_DATA, (T_STR)"int4 int4 int4");

  // Create a message of type XYZ_COORD_DATA 
  msg.Create(mt);

  // Assign it the DATA category for logging 
  TipcSrv::LogAddMt(T_IPC_SRV_LOG_DATA, mt);

  // Set the destination subject of the message 
  msg.Dest((T_STR)"/tutorial/lesson4");

  for (i = 0; i < 30;) {
    // In order to re-use the message, reset the number of the 
    // fields in the message to zero 
    msg.NumFields(0);

    // Build the data part message with 3 integers
    msg << i++;
    msg << i++;
    msg << i++;

    // Send the message 
   rtServer.Send(msg, TRUE);
   rtServer.Flush();
  } // for 

  // Destroy the message 
  msg.Destroy();

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 
