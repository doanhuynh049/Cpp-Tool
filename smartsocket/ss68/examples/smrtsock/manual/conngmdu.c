/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/conngmdu.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* conngmdu.c -- connections GMD example utilities */

#include "conngmd.h"

/* ================================================================ */
/*..cb_conn_msg -- connection msg callback */
void T_ENTRY cb_conn_msg
(
 T_IPC_CONN conn,
 T_IPC_CONN_MSG_CB_DATA data,
 T_CB_ARG arg /* really (T_STR) */
)
{
  T_IPC_MT mt;
  T_STR name;
  T_INT4 seq_num;
  T_STR info;
  
  /* This example callback function is not intended to show any */
  /* useful processing, only how GMD works. */
  TutOut("%s: ", (T_STR)arg);
  
  /* print out the name of the type of the message */
  if (!TipcMsgGetType(data->msg, &mt)) {
    TutOut("Could not get message type from message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  if (!TipcMtGetName(mt, &name)) {
    TutOut("Could not get name from message type: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  TutOut("type %s, ", name);
  
  /* print out the sequence number of the message to show how inner */
  /* workings of GMD operate */
  if (!TipcMsgGetSeqNum(data->msg, &seq_num)) {
    TutOut("Could not get sequence number from msg: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  TutOut("seq_num %d\n", seq_num);
  
  /* print the first field of the message (INFO or GMD_ACK) */
  if (mt == TipcMtLookupByNum(T_MT_INFO)) {
    if (!TipcMsgSetCurrent(data->msg, 0)
	|| !TipcMsgNextStr(data->msg, &info)) {
      TutOut("Could not get field from INFO message: error <%s>.\n",
	     TutErrStrGet());
      return;
    }
    TutOut("  INFO field: %s\n", info);
  }
  else if (mt == TipcMtLookupByNum(T_MT_GMD_ACK)) {
    if (!TipcMsgSetCurrent(data->msg, 0)
	|| !TipcMsgNextInt4(data->msg, &seq_num)) {
      TutOut("Could not get field from GMD_ACK msg: error <%s>.\n",
	     TutErrStrGet());
      return;
    }
    TutOut("  GMD_ACK field: %d\n", seq_num);
  }

} /* cb_conn_msg */

/* ================================================================ */
/*..create_conn_cb -- create example callbacks */
void create_conn_cb
(
 T_IPC_CONN conn
)
{
  /* create callbacks to be executed when certain operations occur */
  TutOut("Create callbacks.\n");
  
  /* create read callback to show received messages */
  if (TipcConnReadCbCreate(conn, (T_IPC_MT) NULL, (T_IPC_CONN_READ_CB_FUNC) cb_conn_msg, "read") == NULL) {
    TutOut("Could not create read cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* create write callback to show sent messages */
  if (TipcConnWriteCbCreate(conn, NULL, cb_conn_msg, "write")
      == NULL) {
    TutOut("Could not create write cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* create default callback to show processed messages */
  if (TipcConnDefaultCbCreate(conn, cb_conn_msg, "default") == NULL) {
    TutOut("Could not create default cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
		}
} /* create_conn_cb */
