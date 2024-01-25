/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/rtcltsnd.cxx#1 $
 */

// From "RTserver and RTclient" chapter in the 
// "SmartSockets C++ Class Library Manual"

// rtcltsnd.cxx -- RTclient example sender

// This sending RTclient creates its connection and sends a data frame
// of messages to a subject (through RTserver).

#include <iostream.h>
#include <rtworks/cxxipc.hxx>
#include "rtclient.hxx"

// ================================================================
//..main -- main program
int main(int argc, char **argv)
{
  T_OPTION option;
  TipcMt mt;

  // Set the option Project to partition ourself.
  option = TutOptionLookup((T_STR)"project");
  if (option == NULL) {
    cout << "Could not lookup the option named project." << endl;
    return T_EXIT_FAILURE;
  }
  if (!TutOptionSetEnum(option, (T_STR)EXAMPLE_PROJECT)) {
    cout << "Could not set the option named project." << endl;
    return T_EXIT_FAILURE;
  }

  // Log outgoing data messages to a message file. Another
  // way to set options is to use TutCommandParseStr.
  TutCommandParseStr((T_STR)"setopt log_out_data log_out.msg");

  // Allow a command-line argument containing the name of a
  // SmartSockets startup command file. This file can be used to set
  // options like Server_Names.  
  if (argc == 2) {
    if (!TutCommandParseFile(argv[1])) {
      cout << "Could not parse startup command file " << argv[1] << endl;
      return T_EXIT_FAILURE;
    }
  }
  else if (argc != 1) { // too many command-line arguments
    cout << "Usage: " << argv[0] << " [ command_file_name ]" << endl;
    return T_EXIT_FAILURE;
  }

  cout << "Creating connection to RTserver." << endl;
  TipcSrv& sndr = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);
  if (!sndr) {
    cout << "Could not create the connection to RTserver." << endl;
    return T_EXIT_FAILURE;
  }

  cout << "Send a frame of data to the subject." << endl;

  cout << "Sending a TIME message." << endl;
  mt.Lookup(T_MT_TIME);
  if (!mt) {
    cout << "Could not lookup TIME message type." << endl;
    return T_EXIT_FAILURE;
  }
  if (!sndr.SrvWrite((T_STR)EXAMPLE_SUBJECT, mt, TRUE,
		     T_IPC_FT_REAL8, 1.0,
		     NULL)) {
    cout << "Could not send TIME message." << endl;
  }

  cout << "Sending a NUMERIC_DATA message." << endl;
  mt.Lookup(T_MT_NUMERIC_DATA);
  if (!mt) {
    cout << "Could not lookup NUMERIC_DATA message type." << endl;
    return T_EXIT_FAILURE;
  }
  if (!sndr.SrvWrite((T_STR)EXAMPLE_SUBJECT, mt, TRUE,
		     T_IPC_FT_STR, "voltage",
		     T_IPC_FT_REAL8, 33.4534,
		     T_IPC_FT_STR, "switch_pos",
		     T_IPC_FT_REAL8, 0.0,
		     NULL)) {
    cout << "Could not send NUMERIC_DATA message." << endl;
  }

  cout << "Sending an END_OF_FRAME message." << endl;
  mt.Lookup(T_MT_END_OF_FRAME);
  if (!mt) {
    cout << "Could not lookup END_OF_FRAME message type." << endl;
    return T_EXIT_FAILURE;
  }
  if (!sndr.SrvWrite((T_STR)EXAMPLE_SUBJECT, mt, TRUE, NULL)) {
    cout << "Could not send END_OF_FRAME message." << endl;
  }

  // Each RTclient automatically creates a connection process
  // callback for CONTROL messages. Use this to send the command
  // "quit force" to the receiver's command interface.
  cout << "Sending a CONTROL message to stop the receiver(s)." << endl;
  mt.Lookup(T_MT_CONTROL);
  if (!mt) {
    cout << "Could not lookup CONTROL message type." << endl;
    return T_EXIT_FAILURE;
  }
  if (!sndr.SrvWrite((T_STR)EXAMPLE_SUBJECT, mt, TRUE,
		     T_IPC_FT_STR, "quit force",
		     NULL)) {
    cout << "Could not send CONTROL message." << endl;
  }

  // Flush the buffered outgoing messages to RTserver.
  if (!sndr.Flush()) {
    cout << "Could not flush outgoing messages to RTserver." << endl;
  }

  // Completely disconnect from RTserver.
  if (!sndr.Destroy(T_IPC_SRV_CONN_NONE)) {
    cout << "Could not fully destroy connection to RTserver." << endl;
  }

  cout << "Sender RTclient process exiting successfully." << endl;
  return T_EXIT_SUCCESS; // all done

} // main  
