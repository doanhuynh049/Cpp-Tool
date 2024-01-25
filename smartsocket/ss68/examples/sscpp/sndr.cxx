/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/sndr.cxx#1 $
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
  }
  catch (TipcSrvException se) {
    Utilities::out("Error in creating connection: %s\n", se.what());
		return T_EXIT_FAILURE;
  }

  // Create a message.
  TipcMsg msg(T_MT_STRING_DATA);
  try {
	  msg.setDest((const char*)"demo");

	  // Build the message with two string fields: "x" and "Hello World". 
	  msg << (const char*)"x" << (const char*)"Hello World";
	  if (!msg) {
		  Utilities::out("Error appending fields of TipcMsg object.\n");
		  return T_EXIT_FAILURE;
	  }
  }
  catch (TipcMsgException me) {
	  Utilities::out("Error in creating and appending message: %s\n", me.what());
    return T_EXIT_FAILURE;
  }

 	// Send the message. 
  try {
    srv->send(msg);
    srv->flush();
    srv->close();
  }
  catch (TipcSrvException se) {
	  Utilities::out("Error in sending message through the connection: %s\n", se.what());
    return T_EXIT_FAILURE;
  }

  return T_EXIT_SUCCESS;
} // main  

