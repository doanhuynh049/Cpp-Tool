/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson4/receive3.c#1 $
 */

/* receive3.c - print out contents of NUMERIC_DATA messages via callback */
/* $RTHOME/examples/smrtsock/tutorial/lesson4/receive3.c */

#include <rtworks/ipc.h>

static void T_ENTRY ProcessNumData(T_IPC_CONN conn, 
				   T_IPC_CONN_PROCESS_CB_DATA data, 
				   T_CB_ARG arg);
static void T_ENTRY DefaultReceiveFunc(T_IPC_CONN conn, 
				       T_IPC_CONN_PROCESS_CB_DATA data, 
				       T_CB_ARG arg);

/* ========================================================================= */
/*..ProcessNumData - callback for processing NUMERIC_DATA messages */
static void T_ENTRY ProcessNumData
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_STR var_name;
  T_REAL8 var_value;

  TutOut("Received NUMERIC_DATA message\n");

   /* Set current field to first field in message */
  TipcMsgSetCurrent(msg, 0);

  /* Traverse Message; print each name-value pair out */
  while (TipcMsgNextStrReal8(msg, &var_name, &var_value)) {
    TutOut( "Var Name = %s; Value = %s\n", var_name, TutRealToStr(var_value));
  } /* while */

} /* ProcessNumData */

/* ========================================================================= */
/*..DefaultReceiveFunc - default callback */
static void T_ENTRY DefaultReceiveFunc
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_IPC_MT mt;
  T_STR name;

  /* Print out the name of the type of the message */
  TipcMsgGetType(msg, &mt);
  TipcMtGetName(mt, &name);
  TutOut("Receive: unexpected message type name is <%s>\n", name);

} /* DefaultReceiveFunc */

/* ========================================================================= */
int main(int argc, char **argv)
{
  T_OPTION option;
  T_IPC_MT mt;

  /* Set the project name */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to the RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Setup callback to catch NUMERIC_DATA messages */
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  TipcSrvProcessCbCreate(mt, ProcessNumData, NULL);

  /* Setup default callback to process anything but NUMERIC_DATA messages */
   TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);

  /* Start subscribing to the "/tutorial/lesson4" subject */
  TipcSrvSubjectSetSubscribe("/tutorial/lesson4", TRUE);

  /* Read and Process all incoming messages */
  TipcSrvMainLoop(T_TIMEOUT_FOREVER);

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
