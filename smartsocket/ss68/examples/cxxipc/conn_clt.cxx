/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/conn_clt.cxx#1 $
 */

// -----------------------------------------------------------------
// The client process connects to the server process and sends two
// messages to the server.
// -----------------------------------------------------------------

// conn_clt.cxx -- connections example client

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// A simple user C++ class encapsulating a write callback   
class MyWriteCallback { 
public:

  MyWriteCallback() {}
  virtual ~MyWriteCallback() {}

  static void T_ENTRY cb_write(T_IPC_CONN conn, 
           T_IPC_CONN_WRITE_CB_DATA data,
           T_CB_ARG arg);
};

// ===================================================================
//..MyWriteCallback::cb_write -- write callback
void T_ENTRY MyWriteCallback::cb_write(T_IPC_CONN conn, 
             T_IPC_CONN_WRITE_CB_DATA data,
             T_CB_ARG arg) 
{
  cout << "Entering cb_write." << endl;
  TipcMsg msg(data->msg);

  // print out the message to the console
  msg.Print(TutOut);
} // cb_write

// ===================================================================
//..main -- main program
int main()
{
  cout << "Creating connection to server process." << endl;

  TipcConnClient client((T_STR)"tcp:_node:5252");

  if (!client) {
    cout << "Could not create connection to server." << endl;
    return T_EXIT_FAILURE;
  }

  // create callbacks to be executed when certain operations occur
  cout << "Create callbacks." << endl;

  // create write callback   
  TipcMt mt_vacant;
  client.WriteCbLookup(mt_vacant, 
           MyWriteCallback::cb_write,
           T_NULL);

  if (!client) {
    client.WriteCbCreate(mt_vacant, 
       MyWriteCallback::cb_write,
       T_NULL);

    if (!client) {
      cout << "Could not create write callback." << endl;
      return T_EXIT_FAILURE;
    }
  }


  cout << "Constructing and sending a NUMERIC_DATA message." << endl;

  TipcMsg msg(T_MT_NUMERIC_DATA);

  if (!msg) {
    cout << "Could not create NUMERIC_DATA messages." << endl;
    return T_EXIT_FAILURE;
  }

  msg << (T_STR)"voltage" << (T_REAL8)33.4534 
      << (T_STR)"switch_pos" << (T_REAL8)0.0;

  if (!msg) { 
    cout << "Could not append fields to NUMERIC_DATA message" << endl;
    return T_EXIT_FAILURE;
  }

  if (!client.Send(msg)) {
    cout << "Could not send NUMERIC_DATA message." << endl;
  }

  cout << "Constructing and sending an INFO message." << endl;

  TipcMt mt(T_MT_INFO);
  if (!mt) {
    cout << "Could not lookup INFO message type." << endl;
    return T_EXIT_FAILURE;
  }

  if (!msg.Type(mt)) {
    cout << "Could not set message type." << endl;
    return T_EXIT_FAILURE;
  }

  if (!msg.NumFields(0)) {
    cout << "Could not set message num fields." << endl;
    return T_EXIT_FAILURE;
  }

  msg << (T_STR)"Now is the time";

  if (!msg) {
    cout << "Could not append fields to INFO message." << endl;
    return T_EXIT_FAILURE;
  }

  if (!client.Send(msg)) {
    cout << "Could not send INFO message." << endl;
    return T_EXIT_FAILURE;
  }

  if (!client.Flush()) {
    cout << "Could not flush buffered outgoing messages" << endl;
    return T_EXIT_FAILURE;
  }

  cout << "Client process exiting successfully." << endl;
  return T_EXIT_SUCCESS;  // all done   

}  // main   
