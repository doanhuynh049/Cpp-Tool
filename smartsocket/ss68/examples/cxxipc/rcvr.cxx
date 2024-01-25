/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/rcvr.cxx#1 $
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
  
  // Subscribe to receive any messages published to the "demo" subject
  srv.SubjectSubscribe((T_STR)"demo", TRUE);

  // Get the next incoming message
  TipcMsg msg(srv.Next(T_TIMEOUT_FOREVER));
  if (!srv) {
    cout << "Error creating TipcMsg object" << endl;
    return T_EXIT_FAILURE;
  }
  // Set the pointer to first field in message
  msg.Current(0);

  T_STR var_name;
  T_STR var_value;

  // Extract the information from the received message
  msg >> var_name >> var_value;
  
  if (!msg) {
    cout << "Error reading fields of TipcMsg object" << endl;
    return T_EXIT_FAILURE;
  }
  
  // Display the values on stdout
  cout << "Variable Name  = " << var_name << endl;
  cout << "Variable Value = " << var_value << endl;
  
  return T_EXIT_SUCCESS;
} // main  
