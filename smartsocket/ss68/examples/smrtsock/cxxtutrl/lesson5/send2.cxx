/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson5/send2.cxx#1 $
 */

// send2.cxx - send a series of INFO messages 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson5/send2.cxx 

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt(T_MT_INFO);

  // Set the name of the project 
  option = TutOptionLookup((T_STR)"project");
  TutOptionSetEnum(option, (T_STR)"smartsockets");

  TutCommandParseStr((T_STR)"setopt unique_subject lesson5_sender");

  // Connect to RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  while(1) {
    rtServer.SrvWrite((T_STR)"/tutorial/lesson5", mt, TRUE,
		    T_IPC_FT_STR, "Hello World!", NULL);
    rtServer.Flush();
    rtServer.MainLoop(1.0);
  } // for 

  return T_EXIT_SUCCESS;
} // main 
