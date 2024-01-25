/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson6/send3.cxx#1 $
 */

// send3.cxx - send a series of MSG_COUNT messages 
// $RTHOME/examples/smrtsock/cxxtutrl/lesson6/send3.cxx 

#define MSG_COUNT 2000
#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main(int argc, char **argv)
{
  TipcMsg msg;
  TipcSrv& rtServer = TipcSrv::Instance();
  TipcMt mt;
  T_INT4 i;

  // Set some options 
  TutCommandParseStr((T_STR)"Setopt project smartsockets");    
  TutCommandParseStr((T_STR)"Setopt unique_subject lesson6_send3");

  // Create a new message type which consists of a single integer. We
  // will use it to hold a message count 
  mt.Create((T_STR)"msg_count", MSG_COUNT, (T_STR)"int4");
 
  // Connect to RTserver 
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  i = 0;
  while (1) {
    i++;
    cout << "Sending message " << i << endl;
    rtServer.SrvWrite((T_STR)"/tutorial/lesson6", mt, TRUE,
		      T_IPC_FT_INT4, i, NULL);
    rtServer.Flush();
    rtServer.MainLoop(1.0);
  } // for 

  return T_EXIT_SUCCESS;
} // main 
