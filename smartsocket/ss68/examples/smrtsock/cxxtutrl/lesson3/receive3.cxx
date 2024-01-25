/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson3/receive3.cxx#1 $
 */

// receive3.cxx - read a NUMERIC_DATA message and print out its contents 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson3/receive3.cxx

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)

{
  T_OPTION option;
  TipcMsg msg;
  T_STR var_name;
  T_REAL8 var_value;
  TipcSrv& rtServer = TipcSrv::Instance();
  
  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "receive3.c: could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Subscribe to receive any messages sent to the "/tutorial/lesson3" subject 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson3", TRUE);

  // This function calls gets the next message from its message queue 

  msg = rtServer.Next(T_TIMEOUT_FOREVER);

  // We now have a message to work with. The following call sets the message
  // pointer (i.e., the current field) to the first field in the data part
  // of the message 
  

  msg.Current(0);

  // Move through the data part of the message; print out each variable name,
  // variable value pair in the message 

  cout << "Contents of NUMERIC_DATA message:" << endl;
  cout << "---------------------------------" << endl;

  while (msg.Next(&var_name, &var_value)) {
      cout << "Var name = " << var_name 
           << ", Var Value = " << TutRealToStr(var_value) << endl;
  } // while 

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver
  
  return T_EXIT_SUCCESS; 
} // main 
