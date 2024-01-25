/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson1/send.cxx#1 $
 */

// send.cxx - send an INFO message to lesson1 subject 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson1/send.cxx 


#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{

  // This one function call will perform the following tasks:
  //	1. Since there was no explicit call to connect to RTserver,
  //       the process will first establish a connection to RTserver
  //       (this might including auto-starting RTserver if it is not
  //       already running)
  //	2. A message of type INFO will be created.
  //	3. The data part of the message will contain the text "Hello World!".
  //	4. The message will be sent to the RTserver process to be delivered to 
  //	   the subject (topic) "/tutorial/lesson1"
  //	5. The message will then be destroyed.
  

  TipcSrv& rtServer = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);
  TipcMt mt(T_MT_INFO);

  rtServer.SrvWrite((T_STR)"/tutorial/lesson1", mt, TRUE, 
   		  T_IPC_FT_STR, "Hello World!", NULL);

  rtServer.Flush(); // This call is necessary to ensure the message is 
                    // immediately sent 

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS; 
} // main 
