/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/conn_clt.cxx#1 $
 */

// -----------------------------------------------------------------
// The client process connects to the server process and sends two
// messages to the server.
// -----------------------------------------------------------------

// conn_clt.cxx -- connections example client

#include <rtworks/sscpp.h>

using namespace SmartSockets;

// A simple user C++ class encapsulating a write callback   
class MyWriteCb
: public MessageCallBack
{
  public:

    virtual void onMessage ( CallBack<MessageCallBack>* callback,
                             TipcMsg & msg,
                             TipcConn & conn)
    {
      try {
        Utilities::out("Entering cb_write.\n");

        // print out the message to the console
        msg.print(TutOut);
      }
      catch (...)
      {
        Utilities::out("Unhandled exception in the callback.");
      }
    }
};

// ===================================================================
//..main -- main program
int main()
{
  TipcConnClient  *client;
  MyWriteCb *wcb  = new MyWriteCb();

  Utilities::out("Creating connection to server process.\n");
  try {
    client = new TipcConnClient((const char*)"tcp:_node:5252");
  }
  catch (TipcConnException connex) {
    Utilities::out("Could not create connection to server. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  // create callbacks to be executed when certain operations occur
  Utilities::out("Create callbacks.\n");

  // create write callback   
  try {
    TipcMt mt_vacant((T_IPC_MT) NULL);
    CallBack<MessageCallBack>* cb = client->writeCbCreate(mt_vacant, wcb,
                                         T_NULL);
  }
  catch (TipcMtException mte) {
    Utilities::out("Could not create message type. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcConnException connex) { 
    Utilities::out("Could not create the write callback. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }
 
  Utilities::out("Constructing and sending a NUMERIC_DATA message.\n");
  try {
    TipcMsg msg(T_MT_NUMERIC_DATA);
    msg << (T_STR)"voltage" << (T_REAL8)33.4534 
        << (T_STR)"switch_pos" << (T_REAL8)0.0;

    client->send(msg);
    client->flush();
  }
  catch (TipcMsgException msge) {
    Utilities::out("Exception in TipcMsg while creating or appending data. %s\n", msge.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception on sending data. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }
  
  Utilities::out("Constructing and sending an INFO message.\n");
  try {
    TipcMsg msg(T_MT_INFO);
    msg.setNumFields(0);
    msg << (const char*)"Now is the time";
    client->send(msg);
    client->flush();
  }
  catch (TipcMsgException msge) {
    Utilities::out("Exception in TipcMsg while creation. %s\n", msge.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcConnException connex) {
    Utilities::out("Exception on sending data. %s\n", connex.what());
    return T_EXIT_FAILURE;
  }

  delete client;

  Utilities::out("Client process exiting successfully.\n");
  return T_EXIT_SUCCESS;  // all done   
}  // main   
