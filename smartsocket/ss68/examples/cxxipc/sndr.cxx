/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/sndr.cxx#1 $
 */

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  // Set the name of the project 
  T_OPTION option = TutOptionLookup((T_STR)"project");
  TutOptionSetStr(option, (T_STR)"ipc_example");

  // Connect to RTserver 
  TipcSrv& srv = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);

  // Create a message 
  TipcMsg msg(T_MT_STRING_DATA);
  msg.Dest((T_STR)"demo");

  // Build the message with two string fields: "x" and "Hello World" 
  msg << (T_STR)"x" << (T_STR)"Hello World";

  if (!msg) {
    cout << "Error appending fields of TipcMsg object" << endl;
    return T_EXIT_FAILURE;
  }

  // Send the message 
  srv.Send(msg);
  srv.Flush();
  srv.Destroy(T_IPC_SRV_CONN_NONE); // force blocking close

  return T_EXIT_SUCCESS;
} // main  
