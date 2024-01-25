/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/rtcltrcv.cxx#1 $
 */

// From "RTserver and RTclient" chapter in the 
// "SmartSockets C++ Class Library Manual"

// rtcltrcv.cxx -- RTclient example receiver

// The receiving RTclient creates its connection to RTserver, and
// receives and processes messages.

#include <rtworks/sscpp.h>

using namespace SmartSockets;

// ===================================================================
//..numcb -- numeric data callback
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

// ================================================================
//..main -- main program
int main(int argc, char **argv)
{
  TipcSrv *rcvr;
  num_cb *ncb = new num_cb();
  def_cb *dcb = new def_cb(); 

  // Set the option Project to partition ourself.
  // Set the name of the project. 
  try {
    Option opt("project");
    opt.setEnum("example");
  }
  catch (OptionException oe) {
    Utilities::out("Error creating options: %s\n", oe.what());
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
      Utilities::out("Could not parse startup command file %s. %s\n", 
                        argv[1], ue.what());
      return T_EXIT_FAILURE;
    }
  }
  else if (argc != 1) { // too many command-line arguments
    Utilities::out("Usage: %s [ command_file_name ]\n", argv[0]);;
    return T_EXIT_FAILURE;
  }
  
  // Connect to RTserver, and open the connection. 
  Utilities::out("Creating connection to RTserver.\n");
  try {
    rcvr = new TipcSrv("", NULL, NULL, NULL);
    rcvr->open();
  }
  catch (TipcSrvException se) {
    Utilities::out("Error in creating connection: %s\n", se.what());
		return T_EXIT_FAILURE;
  }

  // create callbacks to be executed when certain operations occur
  Utilities::out("Create callbacks.\n");
  
  // process callback for NUMERIC_DATA
  try {
    TipcMt mt = TipcMt::lookup(T_MT_NUMERIC_DATA);
    CallBack<MessageCallBack>* cb = rcvr->processCbCreate(mt, ncb);
  }
  catch (TipcMtException mte) {
    Utilities::out("Exception on mt lookup for NUMERIC_DATA. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
  catch (TipcSrvException srve) {
    Utilities::out("Error in creating process callback. %s\n", srve.what());
    return T_EXIT_FAILURE;
  }

  // default callback
  try {
    CallBack<MessageCallBack>* cb = rcvr->defaultCbCreate(dcb);
  }
  catch (TipcSrvException srve) {
    Utilities::out("Error in creating default callback. %s\n", srve.what());
    return T_EXIT_FAILURE;
  }

  try {
    Utilities::out("Start subscribing to standard subjects.\n");
    rcvr->setSubscribeStd(true);

    Utilities::out("Start subscribing to the rcv subject.\n");
    rcvr->setSubscribe((const char*)"rcv", true);
  
    // If an error occurs, then TipcSrv::MainLoop will restart RTserver
    // and return FALSE. We can safely continue.
    for (;;) {
      rcvr->mainLoop(T_TIMEOUT_FOREVER);
    }
  }
  catch (TipcSrvException srve) {
    Utilities::out("Error in TipSrv during Subscribe or in Mainloop. %s\n", srve.what());
  }

  // This line should not be reached.
  Utilities::out("This line should not be reached!!!\n");
  return T_EXIT_FAILURE;

} // main  
