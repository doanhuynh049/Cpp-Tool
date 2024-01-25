/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/conn_srv.cxx#1 $
 */

// ---------------------------------------------------------------------
// This server process waits for a client to connect to it, creates some
// callbacks, and then loops receiving and processing messages.
// ---------------------------------------------------------------------

/// conn_srv.cxx -- connections example server   

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

// -------------------------------------------------------------------
// User handler class for various callbacks
// -------------------------------------------------------------------
class MyCallbacks {
public:
  MyCallbacks() {}
  virtual ~MyCallbacks() {}

  static void T_ENTRY cb_process_numeric_data(T_IPC_CONN conn,
              T_IPC_CONN_PROCESS_CB_DATA data,
              T_CB_ARG arg);

  static void T_ENTRY cb_default(T_IPC_CONN conn,
       T_IPC_CONN_DEFAULT_CB_DATA data,
       T_CB_ARG arg);

  static void T_ENTRY cb_read(T_IPC_CONN conn, 
          T_IPC_CONN_READ_CB_DATA data,
            T_CB_ARG arg);

  static void T_ENTRY cb_queue(T_IPC_CONN conn, 
           T_IPC_CONN_QUEUE_CB_DATA data,
           T_CB_ARG arg);

  static void T_ENTRY cb_error(T_IPC_CONN conn,
           T_IPC_CONN_ERROR_CB_DATA data,
           T_CB_ARG arg);

};

// ================================================================ 
//..MyCallbacks::cb_process_numeric_data -- numeric data callback
void 
T_ENTRY MyCallbacks::cb_process_numeric_data(T_IPC_CONN conn,
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

  // process all the fields of the message   
  while (1) {
    msg >> name >> value;
    if (!msg) {
      break;
    }
    cout << name << " = " << value << endl;
  }

  // make sure we reached the end of the message   
  if (TutErrNumGet() != T_ERR_MSG_EOM) {
    cout << "Did not reach end of message." << endl;
  }

} // cb_process_numeric_data   

// ================================================================ 
//..MyCallbacks::cb_default -- default callback
void T_ENTRY MyCallbacks::cb_default(T_IPC_CONN conn,
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
//..MyCallbacks::cb_read -- read callback
void T_ENTRY MyCallbacks::cb_read(T_IPC_CONN conn, 
        T_IPC_CONN_READ_CB_DATA data,
        T_CB_ARG arg)
{
  cout << "Entering cb_read." << endl;
  TipcMsg msg(data->msg);

  msg.Print(TutOut);
} // cb_read   

// ================================================================
//..MyCallbacks::cb_queue -- queue callback
void T_ENTRY MyCallbacks::cb_queue(T_IPC_CONN conn, 
         T_IPC_CONN_QUEUE_CB_DATA data,
         T_CB_ARG arg)
{
  cout << "Entering cb_queue." << endl;

  TipcMsg msg(data->msg);

  if (!msg) {
    cout << "MyCallbacks::cb_queue() Error constructing TipcMsg" << endl;
    return;
  }

  TipcMt mt(msg.Type());
  if (!mt) {
    cout << "MyCallbacks::cb_queue() Error getting message type" << endl;
    return;
  }

  T_STR name = mt.Name();

  cout << "A message of type " << name << " is being handled" << endl;

  // print out the position of the message being inserted/deleted   
  cout << "A message of type " << name << " is being "
       << (data->insert_flag ? "inserted" : "deleted")
       << " at position " << data->position << endl;

} // cb_queue   

// ================================================================
//..MyCallbacks::cb_error -- error callback
void T_ENTRY MyCallbacks::cb_error(T_IPC_CONN conn,
         T_IPC_CONN_ERROR_CB_DATA data,
         T_CB_ARG arg)
{
  cout << "Entering cb_error." << endl;
  cout << "The error number is " << data->err_num << endl;
} // cb_error   


// ================================================================
//..main -- main program
int main()
{
  TipcMt mt_vacant;

  cout << "Creating server connection to accept clients on." << endl;

  TipcConnServer accepting_server((T_STR)"tcp:_node:5252");
  if (!accepting_server) {
    cout << "Could not create server connection for accepting clients." << endl;
    return T_EXIT_FAILURE;
  }

  cout << "Waiting for client to connect." << endl;

  TipcConn *conn = accepting_server.Accept();

  if (!conn) {
    cout << "Could not accept client." << endl;
    return T_EXIT_FAILURE;
  }

  // create callbacks to be executed when certain operations occur   
  cout << "Create callbacks." << endl;

  // Create a process callback   
  TipcMt mt(T_MT_NUMERIC_DATA);
  if (!mt) {
    cout << "Could not lookup NUMERIC_DATA message type." << endl;
    return T_EXIT_FAILURE;
  }
  
  conn->ProcessCbLookup(mt,
                        MyCallbacks::cb_process_numeric_data,
            NULL);
  
  // check status flag of the object pointed to by conn
  if (!*conn) {
    conn->ProcessCbCreate(mt, 
        MyCallbacks::cb_process_numeric_data,
        NULL);
    if (!*conn) {
      cout << "Could not create NUMERIC_DATA process callback." << endl;
      return T_EXIT_FAILURE;
    }
  }

  // Create a default callback    
  conn->DefaultCbLookup(MyCallbacks::cb_default, NULL);

  if (!*conn) {
    conn->DefaultCbCreate(MyCallbacks::cb_default, NULL);

    if (!*conn) {
      cout << "Could not create default callback." << endl;
      return T_EXIT_FAILURE;
    }
  }

  // create a read callback   
  conn->ReadCbLookup(mt_vacant, MyCallbacks::cb_read, T_NULL);

  if (!*conn) {
    conn->ReadCbCreate(mt_vacant, MyCallbacks::cb_read, T_NULL);

    if (!*conn) {
      cout << "Could not create read callback." << endl;
      return T_EXIT_FAILURE;
    }
  }

  // create a queue callback   
  conn->QueueCbLookup(mt_vacant, MyCallbacks::cb_queue, NULL);

  if (!*conn) {
    conn->QueueCbCreate(mt_vacant, MyCallbacks::cb_queue, NULL);

    if (!*conn) {
      cout << "Could not create queue callback." << endl;
      return T_EXIT_FAILURE;
    }
  }

  // create an error callback   
  conn->ErrorCbLookup(MyCallbacks::cb_error, NULL);

  if (!*conn) {
    conn->ErrorCbCreate(MyCallbacks::cb_error, NULL);

    if (!*conn) {
      cout << "Could not create error callback." << endl;
      return T_EXIT_FAILURE;
    }
  }

  // begin processing messages   
  cout << "Read and process all messages." << endl;

  TipcMsg msg;  // message received and processed   

  for (;;) {
    msg = conn->Next(T_TIMEOUT_FOREVER);
    if (!conn->Status()) {
      break;
    }

    conn->Process(msg);
    if (!conn) {
      cout << "Could not process message." << endl;
    }

    msg.Destroy();
  }
  
   // make sure we reached the end of the data   
  if (TutErrNumGet() != T_ERR_EOF) {
    cout << "Did not reach end of data." << endl;
  }

  delete conn;

  cout << "Server process exiting successfully." << endl;

  return T_EXIT_SUCCESS;  // all done   

}  // main   
