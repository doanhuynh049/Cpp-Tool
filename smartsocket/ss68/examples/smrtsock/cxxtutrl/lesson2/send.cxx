/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson2/send.cxx#1 $
 */

// send.cxx - send INFO message to lesson2 subject 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson2/send.cxx 

#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  T_OPTION option;

  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  // Connect to RTserver 
  TipcSrv& rtServer = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);
  TipcMt mt(T_MT_INFO);

  // Send the message to the "/tutorial/lesson2" subject 
  rtServer.SrvWrite((T_STR)"/tutorial/lesson2", mt , TRUE,
         T_IPC_FT_STR, "Hello World!",
		  NULL);
  rtServer.Flush();

  rtServer.Destroy(T_IPC_SRV_CONN_NONE); // Disconnect from RTserver

  return T_EXIT_SUCCESS;
} // main 

