/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/rtcltrcv.cxx#1 $
 */

// From "RTserver and RTclient" chapter in the 
// "SmartSockets C++ Class Library Manual"

// rtcltrcv.cxx -- RTclient example receiver

// The receiving RTclient creates its connection to RTserver, and
// receives and processes messages.

#include <iostream.h>
#include <rtworks/cxxipc.hxx>
#include "rtclient.hxx"

// ===================================================================
//..cb_process_numeric_data -- numeric data callback
void T_ENTRY cb_process_numeric_data(T_IPC_CONN conn,
			     T_IPC_CONN_PROCESS_CB_DATA data,
			     T_CB_ARG arg)
{
  cout << "Entering cb_process_numeric_data." << endl;

  TipcMsg msg(data->msg); 

  // set current field to first field in message
  msg.Current(0);
  if (!msg.Status()) {
    cout << "Could not set current field of message." << endl;
    return;
  }  

  T_STR name;
  T_REAL8 value;

  while (1) {
    msg >> name >> value;
    if (!msg.Status()) {
      break;
    }
    cout << name << " = " << value << endl;
  }

  // make sure we reached the end of the message
  if (TutErrNumGet() != T_ERR_MSG_EOM) {
    cout << "Did not reach end of message." << endl;
  }

} // cb_process_numeric_data  


// ===================================================================
//..cb_default -- default callback
void T_ENTRY cb_default(T_IPC_CONN conn,
		T_IPC_CONN_DEFAULT_CB_DATA data,
		T_CB_ARG arg)
{
  cout << "Entering cb_default." << endl;

  TipcMsg msg(data->msg); 

  TipcMt mt(msg.Type());
  if (!mt) {
    cout << "Could not get message type from message." << endl;
    return;
  }
  
  T_STR name =  mt.Name();
  if (!mt) {
    cout << "Could not get name from message type." << endl;
    return;
  }

  cout << "Message type name is " << name << endl;

} // cb_default  

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
  TipcSrv& rcvr = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);
  if (!rcvr) {
    cout << "Could not create the connection to RTserver." << endl;
    return T_EXIT_FAILURE;
  }

  // create callbacks to be executed when certain operations occur
  cout << "Create callbacks." << endl;
  
  // process callback for NUMERIC_DATA
  mt.Lookup(T_MT_NUMERIC_DATA);
  if (!mt) {
    cout << "Could not lookup NUMERIC_DATA message type." << endl;
    return T_EXIT_FAILURE;
  }
  if (rcvr.ProcessCbCreate(mt, cb_process_numeric_data, NULL) == NULL) {
    cout << "Could not create NUMERIC_DATA process callback." << endl;
    return T_EXIT_FAILURE;
  }
  
  // default callback
  if (rcvr.DefaultCbCreate(cb_default, NULL) == NULL) {
    cout << "Could not create default callback." << endl;
  }
  
  cout << "Start subscribing to standard subjects." << endl;
  if (!rcvr.StdSubjectSetSubscribe(TRUE, FALSE)) {
    cout << "Could not start subscribing to standard subjects." << endl;
    return T_EXIT_FAILURE;
  }
  
  cout << "Start subscribing to the " << EXAMPLE_SUBJECT << " subject." << endl;
  if (!rcvr.SubjectSubscribe((T_STR)EXAMPLE_SUBJECT, TRUE)) {
    cout << "Could not start subscribing to the " << EXAMPLE_SUBJECT << " subject." << endl;
    return T_EXIT_FAILURE;
  }
  

  // If an error occurs, then TipcSrv::MainLoop will restart RTserver
  // and return FALSE. We can safely continue.
  for (;;) {
    if (!rcvr.MainLoop(T_TIMEOUT_FOREVER)) {
      cout << "TipcSrv::MainLoop failed with error <" << TutErrStrGet() << ">" << endl;
    }
  }
  
  // This line should not be reached.
  cout << "This line should not be reached!!!" << endl;
  return T_EXIT_FAILURE;

} // main  
