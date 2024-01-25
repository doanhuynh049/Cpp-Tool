/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/mon.cxx#1 $
 */

// From "Project Monitoring" chapter in the
// "SmartSockets C++ Class Library Manual"

// mon.cxx -- RTclient example monitoring

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

#define MY_CLIENT_NAME "my_client"

// ===================================================================
//..mon_cb -- print monitoring msg type name
void T_ENTRY mon_cb(T_IPC_CONN conn,
	    T_IPC_CONN_PROCESS_CB_DATA data,
	    T_CB_ARG arg)
{
  TipcMsg msg(data->msg);
  TipcMt  mt(msg.Type());

  cout << (T_STR)arg << " call yields a " << mt.Name() << " response" << endl;
} // mon_cb

// ===================================================================
//..main -- main program
int main()
{
  TipcMt mt;

  TipcSrv& server = TipcSrv::InstanceCreate(T_IPC_SRV_CONN_FULL);

  server.StdSubjectSetSubscribe(TRUE, FALSE);

  mt.Lookup(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  if (!mt) {
    cout << "Message Type lookup failed." << endl;
    return T_EXIT_FAILURE;
  }
  if (server.ProcessCbCreate(mt, mon_cb, 
			     (T_STR)"mon.SubjectSubscribeWatch") == NULL) {
    cout << "Failed to create process callback." << endl;
  }
    
  mt.Lookup(T_MT_MON_CLIENT_TIME_POLL_RESULT);
  if (!mt) {
    cout << "Message Type lookup failed." << endl;
    return T_EXIT_FAILURE;
  }
  if (server.ProcessCbCreate(mt, mon_cb, 
			     (T_STR)"client_mon.TimePoll") == NULL) {
    cout << "Failed to create process callback." << endl;
  }

  mt.Lookup(T_MT_MON_SERVER_GENERAL_POLL_RESULT);
  if (!mt) {
    cout << "Message Type lookup failed." << endl;
    return T_EXIT_FAILURE;
  }
  if (server.ProcessCbCreate(mt, mon_cb, 
			     (T_STR)"server_mon.GeneralPoll") == NULL) {
    cout << "Failed to create process callback." << endl;
  }

  TipcMon mon;
  TipcMonClient client_mon((T_STR)T_IPC_MON_ALL);
  TipcMonServer server_mon((T_STR)T_IPC_MON_ALL);

  if (!mon.SubjectSubscribeWatch((T_STR)"_all", TRUE)) {
    cout << "Failed to setup watch." << endl;
  }
    
  if (!client_mon.TimePoll()) {
    cout << "Failed to poll." << endl;
  }

  if (!server_mon.GeneralPoll()) {
    cout << "Failed to poll." << endl;
  }

  if (!server.MainLoop(10.0)){
    if (TutErrNumGet() != T_ERR_TIMEOUT_REACHED) {
      cout << "Server main loop failed." << endl;
    }
  }

  return T_EXIT_SUCCESS;
} // main
