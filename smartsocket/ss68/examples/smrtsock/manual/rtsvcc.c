/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtsvcc.c#1 $
 */

/* 
 * Mentioned in "RTserver and RTclient" chapter in the 
 * "SmartSockets User's Guide". 
 */

/* rtsvcc.c -- RTclient network service provider example client */

/*
 * This program connects to the RTserver process, submits several
 * messages at once, and then outputs their individual response
 * times.
 */

#include <rtworks/ipc.h>

#define REQUEST_SUBJECT "/rtsvcp/requests"

T_REAL8 job_times[] = { 1.0, 2.0, 7.0, 2.0, 1.0 };
T_INT4  replies_pending;


/* ================================================================ */
/*..cb_process_numeric_data -- process callback for NUMERIC_DATA */
static void T_ENTRY cb_process_numeric_data
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_INT4 i;
  T_STR var_name[5];
  T_REAL8 var_value[5];

  for (i = 0; i < 5; i++) {
    if (!TipcMsgNextStrReal8(data->msg,
                             &var_name[i], &var_value[i])) {
      TutOut("Could not parse NUMERIC_DATA message.\n");
      TutExit(T_EXIT_FAILURE);
    }
  }

  TutOut("Reply (%s %d %s %s %s %s\n",
         var_name[0], (T_INT4)var_value[0],
         var_name[1], TutRealToStr(var_value[1]),
         var_name[2], TutTimeNumToStr(var_value[2]));
  TutOut("       %s %s ",
         var_name[3], TutTimeNumToStr(var_value[3]));
  TutOut("%s %s) elapsed %s seconds\n",
         var_name[4], TutTimeNumToStr(var_value[4]),
         TutRealToStr(var_value[4] - var_value[2]));

  --replies_pending;
  
} /* cb_process_numeric_data */


/* ================================================================ */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_IPC_MT mt;
  T_IPC_MSG msg;
  T_INT4 i;

  TutOut("Creating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create the connection to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  }

  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (T_NULL == mt) {
    TutOut("Could not lookup NUMERIC_DATA message type.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Create a callback to process NUMERIC_DATA messages. */
  if (TipcSrvProcessCbCreate(mt,
			     ((T_IPC_CONN_PROCESS_CB_FUNC)
			      cb_process_numeric_data),
			     T_NULL) == T_NULL) {
    TutOut("Could not create NUMERIC_DATA process callback.\n");
    TutExit(T_EXIT_FAILURE);
  }

  msg = TipcMsgCreate(mt);
  if (T_NULL == msg) {
    TutOut("Could not create NUMERIC_DATA message.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcMsgSetDest(msg, REQUEST_SUBJECT)) {
    TutOut("Could not set message destination.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcMsgSetLbMode(msg, T_IPC_LB_ROUND_ROBIN)) {
    TutOut("Could not set message load balancing mode.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  for (i = 0; i < T_ARRAY_SIZE(job_times); i++) {
    if (!TipcMsgWrite(msg,
                      T_IPC_FT_STR, "message-no",
                      T_IPC_FT_REAL8, (T_REAL8)i + 1.0,
                      T_IPC_FT_STR, "job-time",
                      T_IPC_FT_REAL8, job_times[i],
                      T_IPC_FT_STR, "submit-time",
                      T_IPC_FT_REAL8, TutGetWallTime(),
                      T_NULL)) {
      TutOut("Could not append fields to NUMERIC_DATA message.\n");
      TutExit(T_EXIT_FAILURE);
    }
    
    if (!TipcSrvMsgSend(msg, T_TRUE)) {
      TutOut("Could not send NUMERIC_DATA message.\n");
      TutExit(T_EXIT_FAILURE);
    }

    ++replies_pending;

    if (!TipcMsgSetNumFields(msg, 0)) {
      TutOut("Could not truncate NUMERIC_DATA message.\n");
      TutExit(T_EXIT_FAILURE);
    }
  }

  if (!TipcMsgDestroy(msg)) {
    TutOut("Could not destroy original NUMERIC_DATA message.\n");
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcSrvFlush()) {
    TutOut("Could not flush outgoing messages to RTserver.\n");
  }

  while (replies_pending > 0) {
    if (!TipcSrvMainLoop(5.0)) {
      TutOut("TipcSrvMainLoop failed with error code %d\n",
	     TutErrNumGet());
    }
  }

  TutOut("Sending a CONTROL message to stop service providers.\n");
  mt = TipcMtLookupByNum(T_MT_CONTROL);
  if (T_NULL == mt) {
    TutOut("Could not lookup CONTROL message type.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcSrvMsgWrite(REQUEST_SUBJECT, mt, T_TRUE,
		       T_IPC_FT_STR, "shutdown",
		       T_NULL)) {
    TutOut("Could not send CONTROL message.\n");
  }

  TutOut("Destroying connection to server process.\n");
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Could not destroy connection to server process.\n");
    TutExit(T_EXIT_FAILURE);
  }

  return T_EXIT_SUCCESS;
  
} /* main */
