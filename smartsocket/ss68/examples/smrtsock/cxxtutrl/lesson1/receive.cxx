/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson1/receive.cxx#1 $
 */

// receive.cxx - read and print an INFO message 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson1/receive.cxx
 
#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  TipcSrv& rtServer = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);
  TipcMsg msg;
  T_STR msg_text;

  // The following function call performs two tasks:
  //    1. Since there was no explicit call to connect to RTserver,  
  //   	   this process will first establish a connection to RTserver
  //  	   (this might include auto-starting RTserver).
  //	2. Inform RTserver to forward any message which has
  //       been sent to the "/tutorial/lesson1" subject by any client 
  //       in the project.
  
 
  rtServer.SubjectSubscribe((T_STR)"/tutorial/lesson1", TRUE);

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

  // The next call gets the string field from the data part of the message 
  
  msg >> msg_text;
  
  // Finally, we print out the text we have retrieved from the message   

  cout << "Text from INFO message = " << msg_text << endl;

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS; 
} // main 

