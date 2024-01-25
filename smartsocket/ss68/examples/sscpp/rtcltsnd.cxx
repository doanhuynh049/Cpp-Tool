/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/rtcltsnd.cxx#1 $
 */

// From "RTserver and RTclient" chapter in the 
// "SmartSockets C++ Class Library Manual"

// rtcltsnd.cxx -- RTclient example sender

// This sending RTclient creates its connection and sends a data frame
// of messages to a subject (through RTserver).

#include <rtworks/sscpp.h>

using namespace SmartSockets;

// ================================================================
//..main -- main program
int main(int argc, char **argv)
{
  TipcSrv *sndr;

  // Set the name of the project. 
  try {
    Option opt("project");
    opt.setEnum("example");
  }
  catch (OptionException oe) {
    Utilities::out("Error creating options: %s\n", oe.what());
    return T_EXIT_FAILURE;
  }

  // Log outgoing data messages to a message file. Another
  // way to set options is to use TutCommandParseStr.
  try {
    Utilities::parseCommandString((const char*)"setopt log_out_data log_out.msg");
  }
  catch (UtilitiesException ue) {
    Utilities::out("Could not parse startup command file %s. %s\n", 
                    argv[1], ue.what());
    return T_EXIT_FAILURE;
  }

  // Allow a command-line argument containing the name of a
  // SmartSockets startup command file. This file can be used to set
  // options like Server_Names.  
  if (argc == 2) {
    try {
      Utilities::parseCommandFile((const char*)argv[1]);
    }
    catch (UtilitiesException ue) {
      Utilities::out("Could not parse startup command file %s. %s\n", argv[1], ue.what());
      return T_EXIT_FAILURE;
    }
  }
  else if (argc != 1) { // too many command-line arguments
    Utilities::out("Usage: %s [ command_file_name ]\n", argv[0]);
    return T_EXIT_FAILURE;
  }

  // Connect to RTserver, and open the connection. 
  Utilities::out("Creating connection to RTserver.\n"); 
  try {
    sndr = new TipcSrv("", NULL, NULL, NULL);
    sndr->open();
  }
  catch (TipcSrvException se) {
    Utilities::out("Error in creating connection: %s\n", se.what());
		return T_EXIT_FAILURE;
  }

  Utilities::out("Send a frame of data to the subject.\n");

  Utilities::out("Sending a TIME message.\n");
  try {
    TipcMt mt = TipcMt::lookup(T_MT_TIME);
    sndr->write((const char*)"rcv", mt, true, T_IPC_FT_REAL8, 1.0, NULL);
  }
  catch (TipcMtException mte) {
    Utilities::out("Could not lookup TIME message type. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcSrvException srve) {
    Utilities::out("Could not send TIME message. %s\n", srve.what());
  }

  Utilities::out("Sending a NUMERIC_DATA message.\n");
  try {
    TipcMt mt = TipcMt::lookup(T_MT_NUMERIC_DATA);
    sndr->write((const char*)"rcv", mt, true, 
		                 T_IPC_FT_STR, "voltage",
		                 T_IPC_FT_REAL8, 33.4534,
		                 T_IPC_FT_STR, "switch_pos",
		                 T_IPC_FT_REAL8, 0.0,
                     NULL);
  }
  catch (TipcMtException mte) {
    Utilities::out("Could not lookup NUMERIC_DATA message type. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcSrvException srve) {
    Utilities::out("Could not send NUMERIC_DATA message. %s\n", srve.what());
  }

  Utilities::out("Sending an END_OF_FRAME message.\n");
  try {
    
    TipcMt mt = TipcMt::lookup(T_MT_END_OF_FRAME);
    sndr->write((const char*)"rcv", mt, true, NULL);
  }
  catch (TipcMtException mte) {
    Utilities::out("Could not lookup END_OF_FRAME message type. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcSrvException srve) {
    Utilities::out("Could not send END_OF_FRAME message. %s\n", srve.what());
  }
  
  // Each RTclient automatically creates a connection process
  // callback for CONTROL messages. Use this to send the command
  // "quit force" to the receiver's command interface.
  Utilities::out("Sending a CONTROL message to stop the receiver(s).\n");
  try {
    TipcMt mt = TipcMt::lookup(T_MT_CONTROL);
    sndr->write((const char*)"rcv", mt, true, T_IPC_FT_STR, (const char*) "quit force", NULL);
  }
  catch (TipcMtException mte) {
    Utilities::out("Could not lookup CONTROL message type. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcSrvException srve) {
    Utilities::out("Could not send CONTROL message. %s\n", srve.what());
  }

  // Flush the buffered outgoing messages to RTserver.
  try {
    sndr->flush();
    sndr->close();
  }
  catch (TipcSrvException srve) {
    Utilities::out("Error in TipcSrv while flushing and closing the connection. %s\n", srve.what());
  }

  Utilities::out("Sender RTclient process exiting successfully.\n");
  return T_EXIT_SUCCESS; // all done
} // main  
