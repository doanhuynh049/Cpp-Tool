/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/conn_srv.cxx#1 $
 */

// ---------------------------------------------------------------------
// This server process waits for a client to connect to it, creates some
// callbacks, and then loops receiving and processing messages.
// ---------------------------------------------------------------------

/// conn_srv.cxx -- connections example server   

#include <rtworks/sscpp.h>

using namespace SmartSockets;

// -------------------------------------------------------------------
// User handler class for various callbacks
// -------------------------------------------------------------------

// ================================================================ 
//..MyCallbacks::cb_process_numeric_data -- numeric data callback
class num_cb
: public MessageCallBack
{
  public:

    virtual void onMessage ( CallBack<MessageCallBack>* callback,
                             TipcMsg & msg,
                             TipcConn & conn)
    {
      Utilities::out("Entering num_cb.\n");
      const char  *name;
      real8 value;

      msg.setCurrent(0);
      try {
        while (1) {
          msg >> name >> value;
        }
      }
      catch (TipcMsgException msge) {
        Utilities::out("%s = %f\n", name, value);
      }
      catch (...)
      {
        Utilities::out("Unhandled exception in the num_cb.");
      }
      
      // make sure we reached the end of the message
      if (TutErrNumGet() != T_ERR_MSG_EOM) {
        Utilities::out("Did not reach end of message.\n");
      }
    }

};

// ===================================================================
//..cb_default -- default callback
class def_cb
: public MessageCallBack
{
  public:

    virtual void onMessage ( CallBack<MessageCallBack>* callback,
                                       TipcMsg & msg,
                                       TipcConn & conn)
    {
      Utilities::out("Entering def_cb.\n");
      try {
        TipcMt mt(msg.getType());
        char *name =  (char *)mt.getName();
        Utilities::out("Message type name is %s\n", name);
      }
      catch (TipcMtException mte) {
        Utilities::out("Error in TipcMt. %s\n", mte.what());
      }
      catch (...) {
        Utilities::out("Unhandled exception in the def_cb.");
      }
    }
};

// A simple user C++ class encapsulating a read callback   
class read_cb
: public MessageCallBack
{
  public:

    virtual void onMessage ( CallBack<MessageCallBack>* callback,
                             TipcMsg & msg,
                             TipcConn & conn)
    {
      try {
        Utilities::out("Entering read_cb.\n");

        // print out the message to the console
        msg.print(TutOut);
      }
      catch (...)
      {
        Utilities::out("Unhandled exception in the read_cb.");
      }
    }
};
   
class queue_cb
: public MessageQueueCallBack
{
  public:

    virtual void onMessage(CallBack<MessageQueueCallBack> * callback,
                           TipcMsg & msg,
                           TipcConn & connection,
                           uint4 queuePos,
                           bool is_insert )
    {
      Utilities::out("Entering cb_queue.\n");

      try {
        TipcMt mt(msg.getType());
        char *name =  (char *)mt.getName();
        Utilities::out("A message type %s is being %s at position %d\n", 
                name, (is_insert ? "inserted" : "deleted"), queuePos);
      }
      catch (...) {
        Utilities::out("Unhandled exception in the queue_cb.");
      }
    }
};

class error_cb
: public ErrorCallBack
{
  public:

    virtual void onError ( CallBack < ErrorCallBack > * callBack,
                                Exception& e, TipcConn& conn)
    {
      Utilities::out("Entering cb_error.\n");
      Utilities::out("The error is %s\n", e.what());
    }
};

//================================================================
//..main -- main program
int main()
{
  TipcConn *conn;
  TipcMt mt_vacant((T_IPC_MT)NULL);
  num_cb *ncb   = new num_cb();
  def_cb *dcb   = new def_cb();
  read_cb *rcb  = new read_cb();
  queue_cb *qcb = new queue_cb();
  error_cb *ecb = new error_cb();

  try {
    Utilities::out("Creating server connection to accept clients on.\n");
    TipcConnServer accepting_server((T_STR)"tcp:_node:5252");
  
    Utilities::out("Waiting for client to connect.\n");
    conn = accepting_server.accept();
  }
  catch (TipcConnException connex) {
    Utilities::out("Could not create server connection for accepting clients. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  // create callbacks to be executed when certain operations occur   
  Utilities::out("Create callbacks.\n");

  // Create a process callback   
  try {
    TipcMt mt =  TipcMt(T_MT_NUMERIC_DATA);
    conn->processCbCreate(mt, ncb);
  }
  catch (TipcMtException mte) {
    Utilities::out("Exception creating msg type. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception creating process callback. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  try {
    conn->defaultCbCreate(dcb);
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception creating default callback. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  // create a read callback   
  try {
    conn->readCbCreate(mt_vacant, rcb, T_NULL);
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception creating read callback. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }
 
  // create a queue callback   
  try {
    conn->queueCbCreate(mt_vacant, qcb);
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception creating queue callback. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  // create an error callback   
  try {
    conn->errorCbCreate(ecb);
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception creating error callback. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  // begin processing messages   
  Utilities::out("Read and process all messages.\n");
  try {
    for (;;) {
      TipcMsg msg;
      conn->next(msg, T_TIMEOUT_FOREVER);
      conn->process(msg);
    }
  }
  catch (TipcMsgException msge) {
    Utilities::out("Exception in tipcmsg. %s\n", msge.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception on connection. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }
  
  // make sure we reached the end of the data   
  if (TutErrNumGet() != T_ERR_EOF) {
    Utilities::out("Did not reach end of data.\n");
  }

  delete conn;

  Utilities::out("Server process exiting successfully.\n");
  return T_EXIT_SUCCESS;  // all done   

}  // main   
