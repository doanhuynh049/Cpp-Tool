/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson3/receive.cxx#1 $
 */

// receive.cxx - read an INFO message and print out its contents 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson3/receive.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  T_STR msg_text;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt(T_MT_INFO);
  
  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "receive.cxx: could not connect to Rtserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Subscribe to receive any messages sent to the "/tutorial/lesson3" subject 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson3", TRUE);

  // This function calls gets the next message from its message queue;
  // The T_TIMEOUT_FORVER constant specifies that the function will block 
  // (wait) forever for a message. You can specify an actual time-out period
  // for this argument; for example, 10.0 would mean the function would
  // return with a failure code after 10 seconds.
  

  msg = rtServer.Next(T_TIMEOUT_FOREVER);

  // We now have a message to work with. The following call sets the message
  // pointer (i.e., the current field) to the first field in the data part
  // of the message 
  

  msg.Current(0);

  // The next call gets the string field from the data part
  // of the message.  
  

  msg >> msg_text;
  
  // Finally, we print out the infromation we have retrieved from the
  // message
  
  
  cout << "Text from INFO message = " << msg_text << endl;

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 

