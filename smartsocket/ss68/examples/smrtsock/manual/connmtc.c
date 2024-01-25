/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/connmtc.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* connmtc.c -- multithreaded connections example client */

/*
 * This program connects to the server process, submits several messages 
 * at once, and then outputs their individual response times.
 */

#include <rtworks/ipc.h>

#define T_NUM_JOB_TIMES 5
T_REAL8 job_times[] = { 1.0, 2.0, 7.0, 2.0, 1.0 };

/* ================================================================ */
/*..cb_process_numeric_data -- process callback for NUMERIC_DATA */
void T_ENTRY cb_process_numeric_data
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_INT4 i;
  T_STR var_name[5];
  T_REAL8 var_value[5];
  
  /* set current field to first field in message */
  if (!TipcMsgSetCurrent(data->msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  for (i = 0; i < 5; i++) {
    if (!TipcMsgNextStrReal8(data->msg,
                             &var_name[i], &var_value[i])) {
      TutOut("Could not parse NUMERIC_DATA message: error <%s>.\n",
	     TutErrStrGet());
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
  
} /* cb_process_numeric_data */

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_IPC_CONN conn;
  T_IPC_MT mt;
  T_IPC_MSG msg;
  T_INT4 i;
  
  TutOut("Creating connection to server process.\n");
  conn = TipcConnCreateClient("tcp:_node:4000");
  if (NULL == conn) {
    TutOut("Could not connect to server process: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (NULL == mt) {
    TutOut("Could not look up NUMERIC_DATA msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  if (TipcConnProcessCbCreate(conn, mt,
			      cb_process_numeric_data,
			      NULL) == NULL) {
    TutOut("Could not create NUMERIC_DATA process cb: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  msg = TipcMsgCreate(mt);
  if (NULL == msg) {
    TutOut("Could not create NUMERIC_DATA message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  for (i = 0; i < T_NUM_JOB_TIMES; i++) {
    if (!TipcMsgWrite(msg,
                      T_IPC_FT_STR, "message-no",
                      T_IPC_FT_REAL8, (T_REAL8)i + 1.0,
                      T_IPC_FT_STR, "job-time",
                      T_IPC_FT_REAL8, job_times[i],
                      T_IPC_FT_STR, "submit-time",
                      T_IPC_FT_REAL8, TutGetWallTime(),
                      NULL)) {
      TutOut("Could not append to NUMERIC_DATA msg: error <%s>.\n",
	     TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
    
    if (!TipcConnMsgSend(conn, msg)) {
      TutOut("Could not send NUMERIC_DATA message: error <%s>.\n",
	     TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
    
    if (!TipcMsgSetNumFields(msg, 0)) {
      TutOut("Could not truncate NUMERIC_DATA message: error <%s>.\n",
	     TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
  }
  
  if (!TipcMsgDestroy(msg)) {
    TutOut("Could not destroy NUMERIC_DATA msg: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcConnFlush(conn)) {
    TutOut("Could not flush connection to server: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcConnMainLoop(conn, 15.0)) {
    TutOut("Could not run main loop to server: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Destroying connection to server process.\n");
  if (!TipcConnDestroy(conn)) {
    TutOut("Could not destroy connection to server: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  return T_EXIT_SUCCESS;
} /* main */
