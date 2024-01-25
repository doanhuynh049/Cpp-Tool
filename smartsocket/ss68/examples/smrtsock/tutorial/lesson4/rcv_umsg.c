/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson4/rcv_umsg.c#1 $
 */

/*
 * rcv_umsg.c - print out contents of messages of user-defined type 
 * XYZ_COORD_DATA
 */
/* $RTHOME/examples/smrtsock/tutorial/lesson4/rcv_umsg.c */

#include <rtworks/ipc.h>
#define XYZ_COORD_DATA 1001

static void T_ENTRY ProcessXYZ(T_IPC_CONN conn, 
			       T_IPC_CONN_PROCESS_CB_DATA data, 
                               T_CB_ARG arg);

static void T_ENTRY DefaultReceiveFunc(T_IPC_CONN conn, 
                                       T_IPC_CONN_PROCESS_CB_DATA data, 
				       T_CB_ARG arg);

/* ========================================================================= */
/*..ProcessXYZ - callback for processing XYZ_COORD_DATA messages */
static void T_ENTRY ProcessXYZ
(
 T_IPC_CONN conn, 
 T_IPC_CONN_PROCESS_CB_DATA data, 
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_INT4 field_value;

  TutOut("Received XYZ_COORD_DATA message\n");

   /* Set current field to first field in message */
  TipcMsgSetCurrent(msg, 0);

  /* Traverse Message; print value from each field */
  TipcMsgNextInt4(msg, &field_value);
  TutOut("Field 1 Value = %d\n", field_value);  
  TipcMsgNextInt4(msg, &field_value);
  TutOut("Field 2 Value = %d\n", field_value);
  TipcMsgNextInt4(msg, &field_value);
  TutOut("Field 3 Value = %d\n", field_value);
} /* ProcessXYZ */

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

  /* Define a new message type */
  mt = TipcMtCreate("xyz_coord_data", XYZ_COORD_DATA, 
		    "int4 int4 int4");

  /* Assign new type to the DATA category for logging */
  TipcSrvLogAddMt(T_IPC_SRV_LOG_DATA, mt);

  /* Setup callback to process new message type */
  TipcSrvProcessCbCreate(mt, ProcessXYZ, NULL);

  /* Setup default callback to process anything but XYZ_COORD_DATA messages */
  TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);

  /* Start subscribing to the "/tutorial/lesson4" subject */
  TipcSrvSubjectSetSubscribe("/tutorial/lesson4", TRUE);

  /* Read and process all incoming messages */
  TipcSrvMainLoop(T_TIMEOUT_FOREVER);

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
