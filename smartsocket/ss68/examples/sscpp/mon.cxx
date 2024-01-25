/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/mon.cxx#1 $
 */

// From "Project Monitoring" chapter in the
// "SmartSockets C++ Class Library Manual"

// mon.cxx -- RTclient example monitoring

#include <rtworks/sscpp.h>

using namespace SmartSockets;

// ===================================================================
//..mon_cb -- print monitoring msg type name
class mon_cb
: public MessageCallBack
{
  public:

    virtual void onMessage ( CallBack<MessageCallBack>* callback,
                             TipcMsg & msg,
                             TipcConn & conn)
    {
      msg.setCurrent(0);
      const char* data = (const char*) callback->getArgument();
      char *str = new char[1024];
      strcpy(str, data);
      strcat(str, "type = ");
      strcat(str, msg.getType().getName());
      try
      {
        strcat(str, msg.nextString());
        Utilities::out("from Cb routine: %s. \n", str);
        delete []str;
      }
      catch(...)
      {
        delete []str;
        Utilities::out("Unhandled exception in the callback.");
      }
    }

};

// ===================================================================
//..main -- main program
int main()
{
  TipcSrv *server;
  mon_cb *mcb = new mon_cb();

  // Connect to RTserver, and open the connection. 
  try {
    server = new TipcSrv("", NULL, NULL, NULL);
    server->open();
    server->setSubscribeStd(true);
  }
  catch (TipcSrvException se) {
    Utilities::out("Error in creating connection: %s\n", se.what());
		return T_EXIT_FAILURE;
  }

  
  try {
    TipcMt mt = TipcMt::lookup(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
    CallBack<MessageCallBack>* cb = server->processCbCreate(mt, mcb,
                                   (void*) "mon.SubjectSubscribeWatch");
  }
  catch (TipcMtException mte) {
    Utilities::out("Exception on SubjectSubscribeWatch cb. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }
    
  try {
    TipcMt mt = TipcMt::lookup(T_MT_MON_CLIENT_TIME_POLL_RESULT);
    CallBack<MessageCallBack>* cb = server->processCbCreate(mt, mcb,
                                   (void*) "mon.clientTimePoll");
  }
  catch (TipcMtException mte) {
    Utilities::out("Exception on ClientTimePoll cb. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }

  try {
    TipcMt mt = TipcMt::lookup(T_MT_MON_SERVER_GENERAL_POLL_RESULT);
    CallBack<MessageCallBack>* cb = server->processCbCreate(mt, mcb,
                                   (void*) "mon.serverGeneralPoll");
  }
  catch (TipcMtException mte) {
    Utilities::out("Exception on ServerGeneralPoll cb. %s\n", mte.what());
    return T_EXIT_FAILURE;
  }

  TipcMon mon(*server);
  try {
    mon.subjectSubscribeSetWatch((const char*)T_IPC_MON_ALL, true);
  }
  catch (TipcMonException monex) {
    Utilities::out("Failed to setup watch. %s\n", monex.what());
  }
    
  try {
    mon.clientTimePoll((const char*)T_IPC_MON_ALL);

    mon.serverGeneralPoll((const char*)T_IPC_MON_ALL);
  }
  catch (TipcMonException monex) {
    Utilities::out("Failed to poll. %s\n", monex.what());
  }

  try {
    server->mainLoop(10.0);
  }
  catch (TipcSrvException se) {
    if (se.getErrNum() != T_ERR_TIMEOUT_REACHED) {
      Utilities::out("Server main loop failed.\n");
    }
    
    Utilities::out("Exception in mainloop. %s\n", se.what());
  }

  server->close(); 
  return T_EXIT_SUCCESS;
} // main
