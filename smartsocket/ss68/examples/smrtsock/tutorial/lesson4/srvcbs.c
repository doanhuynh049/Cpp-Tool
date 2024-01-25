/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson4/srvcbs.c#1 $
 */

/* srvcbs.c - shows example uses of server create/destroy callbacks */
/* $RTHOME/examples/smrtsock/tutorial/lesson4/srvcbs.c */

#include <rtworks/ipc.h>

#define MAX_PASSWD_LENGTH 20

static void T_ENTRY server_connect(T_IPC_CONN conn,
					T_IPC_SRV_CREATE_CB_DATA data,
					T_CB_ARG arg);
static void T_ENTRY server_disconnect(T_IPC_CONN conn,
					T_IPC_SRV_DESTROY_CB_DATA data,
					T_CB_ARG arg);

/* ================================================================ */
/*..server_connect - ask for password when connecting to RTserver */
static void T_ENTRY server_connect
(
 T_IPC_CONN conn,
 T_IPC_SRV_CREATE_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR password_correct = "smartsockets\n";
  T_CHAR password_entered[MAX_PASSWD_LENGTH];

  /* Prompt for password when trying to connect to RTserver */
  TutOut("Connecting to RTserver...\n");
  TutOut("Please enter password : ");
  fgets(password_entered, MAX_PASSWD_LENGTH, stdin);

  /* If password is correct, let them connect to RTserver, 
     otherwise disconnect from RTserver and exit program */
  if (!strcmp(password_entered, password_correct)) {
    /* Passwords matched */
    TutOut( "Password accepted!\n");
  } else {
    /* Password did not match */
    TutOut("Passwords is not correct!\n");
    TutOut("You are being disconnected from RTserver\n");
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
    TutExit(T_EXIT_FAILURE);
  } /* else */

} /* server_connect */

/* ================================================================ */
/*..server_disconnect - callback for server destroy events */
static void T_ENTRY server_disconnect
(
 T_IPC_CONN conn,
 T_IPC_SRV_DESTROY_CB_DATA data,
 T_CB_ARG arg
)
{
  TutOut("...Disconnecting from RTserver\n");
} /* server_disconnect */

/* ================================================================ */
int main(int argc, char *argv)
{
  /* Use the start_always prefix to auto-start an RTserver if necessary */
  TutCommandParseStr("setopt server_names start_always:_node");

  /* Setup server create callback */
  TipcSrvCreateCbCreate(server_connect, NULL);

  /* Setup server destroy callback */
  TipcSrvDestroyCbCreate(server_disconnect, NULL);

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Read and process all incoming messages */
  while (1) {
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);
  } /* while */
	
  /* Disconnect from RTserver*/
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
