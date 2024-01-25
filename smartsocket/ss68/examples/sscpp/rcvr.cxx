/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/rcvr.cxx#1 $
 */

#include <rtworks/sscpp.h>

using namespace SmartSockets;

int main(int argc, char **argv)
{
  TipcSrv *srv;

  // Set the name of the project. 
  try {
    Option opt("project");
    opt.setEnum("ipc_example");
  }
  catch (OptionException oe) {
    Utilities::out("Error creating options: %s\n", oe.what());
	  return T_EXIT_FAILURE;
  }

  // Connect to RTserver, and open the connection. 
  try {
	  srv = new TipcSrv("", NULL, NULL, NULL);
	  srv->open();

	  // Subscribe to receive any messages published to the "demo" subject.
	  srv->setSubscribe((const char*)"demo");
  }
  catch (TipcSrvException se)
  {
	  Utilities::out("Error in creating connection: %s\n", se.what());
	  return T_EXIT_FAILURE;
  }
   
  const char *var_name;
  const char *var_value;

  // Get the next incoming message.
  try {
    TipcMsg msg;
    if(!srv->nextEx(msg, T_TIMEOUT_FOREVER))
    {
  	  Utilities::out("Timeout reached.\n");
      return T_EXIT_FAILURE;
    }
  
    if (!msg) {
      Utilities::out("Error creating TipcMsg object.\n");
      return T_EXIT_FAILURE;
    }

    // Set the pointer to first field in message
    msg.setCurrent(0); 

    // Extract the information from the received message.
    msg >> var_name >> var_value;
    if (!msg) {
      Utilities::out("Error reading fields of TipcMsg object.\n");
      return T_EXIT_FAILURE;
    }
  }
  catch(TipcMsgException me) {
	  Utilities::out("Error in TipMsg class: %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  
  // Display the values on stdout.
  Utilities::out("Variable Name  = %s\n", var_name);
  Utilities::out("Variable Value = %s\n", var_value);

  srv->close();
  return T_EXIT_SUCCESS;
} // main  

