/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/rcvrcb.cxx#1 $
 */

#include <rtworks/sscpp.h>

using namespace SmartSockets;

// ===================================================================
//..msgcb -- callback

class msg_cb
: public MessageCallBack
{
  public:

    virtual void onMessage ( CallBack<MessageCallBack>* callback,
                             TipcMsg & msg,
                             TipcConn & conn)
    {

      Utilities::out("Entering msg_cb.\n");
      const char *var_name;
      const char *var_value;

      try {
        msg.setCurrent(0);
        // Extract the information from the received message.
        msg >> var_name >> var_value;
        if (!msg) {
          Utilities::out("Error reading fields of TipcMsg object.\n");
        }
      }
      catch(TipcMsgException me) {
	      Utilities::out("Error in TipMsg class: %s\n", me.what());
      }
  
      // Display the values on stdout.
      Utilities::out("Variable Name  = %s\n", var_name);
      Utilities::out("Variable Value = %s\n", var_value);
    }
};

int main(int argc, char **argv)
{
  TipcSrv *srv;
  msg_cb  *mcb = new msg_cb();
  CallBack<MessageCallBack>* cb;

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
   
  // process callback for STRING_DATA
  try {
    TipcMt mt = TipcMt::lookup(T_MT_STRING_DATA);
    cb = srv->processCbCreate(mt, mcb);
  }
  catch (TipcMtException mte) {
    Utilities::out("Exception on mt lookup for STRING_DATA. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcSrvException srve) {
    Utilities::out("Error in creating process callback. %s\n", srve.what());
    return T_EXIT_FAILURE;
  }

  try {
    srv->mainLoop(45.0);
  }
  catch (TipcSrvException se) {
    if (se.getErrNum() != T_ERR_TIMEOUT_REACHED) {
      Utilities::out("Server main loop failed.\n");
    }
    
    Utilities::out("Exception in mainloop. %s\n", se.what());
  }

  cb->destroy();
  delete mcb;

  srv->close(); 
  return T_EXIT_SUCCESS;
} // main  
