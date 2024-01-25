/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/cxxtutrl/lesson4/srvcbs.cxx#1 $
 */

// srvcbs.cxx - shows example uses of server create/destroy callbacks
// $RTHOME/examples/smrtsock/cxxtutrl/lesson4/srvcbs.cxx
 
 #include <iostream.h>
#include <rtworks/cxxipc.hxx>

#define MAX_PASSWD_LENGTH 20
TipcSrv& rtServer = TipcSrv::Instance();

// ================================================================
//..server_connect - ask for password when connecting to RTserver
static void T_ENTRY server_connect(T_IPC_CONN conn, 
                           T_IPC_SRV_CREATE_CB_DATA data, 
                           T_CB_ARG arg)
{
  T_STR password_correct = (T_STR)"smartsockets";
  T_CHAR password_entered[MAX_PASSWD_LENGTH];

  // Prompt for password when trying to connect to RTserver
  cout << "Connecting to RTserver..." << endl;
  cout << "Please enter password : " << endl;
  cin >> password_entered;

  // If password is correct, let them connect to RTserver, 
  // otherwise disconnect from RTserver and exit program
  if (!strcmp(password_entered, password_correct)) {
    // Passwords matched
    cout << "Password accepted!" << endl;
  } else {
    // Password did not match
    cout << "Passwords is not correct!" << endl;
    cout << "You are being disconnected from RTserver" << endl;
    rtServer.Destroy(T_IPC_SRV_CONN_NONE);
    TutExit(T_EXIT_FAILURE);
  } // else

} // server_connect

// ================================================================
//..server_disconnect - callback for server destroy events
static void T_ENTRY server_disconnect(T_IPC_CONN conn, 
                              T_IPC_SRV_DESTROY_CB_DATA data, 
                              T_CB_ARG arg)
{
  cout << "...Disconnecting from RTserver" << endl;
} // server_disconnect

// ================================================================
int main(int argc, char **argv)
{
  // TipcSrv& rtServer = TipcSrv::Instance();

  // Setup server create callback
  rtServer.CreateCbCreate(server_connect, NULL);

  // Setup server destroy callback
  rtServer.DestroyCbCreate(server_disconnect, NULL);

  // Connect to RTserver
  if (!rtServer.Create(T_IPC_SRV_CONN_FULL)) {
    cout << "Could not connect to RTserver!" << endl;
    TutExit(T_EXIT_FAILURE);
  } // if 

  // Read and process all incoming messages
  while (1) {
    rtServer.MainLoop(T_TIMEOUT_FOREVER);
  } // while

  return T_EXIT_SUCCESS;
} // main
