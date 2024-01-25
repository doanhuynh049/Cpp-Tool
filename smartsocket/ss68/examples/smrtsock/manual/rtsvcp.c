/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtsvcp.c#1 $
 */

/* 
 * Mentioned in "RTserver and RTclient" chapter in the 
 * "SmartSockets User's Guide". 
 */

/* rtsvcp.c -- RTclient network service provider example */

/*
 * This RTclient process demonstrates one possible architecture for
 * a network service provider.  If threads are supported on the target
 * architecture, they are used to allow the service to process several
 * requests concurrently.
 */

#include <rtworks/ipc.h>

#define REQUEST_SUBJECT "/rtsvcp/requests"
#define HELPER_COUNT 2
#define SHUTDOWN_CHECK_INTERVAL 5.0

T_TSD_KEY id_key = T_INVALID_TSD_KEY;
T_BOOL shutdown_in_progress  = T_FALSE;


/* ================================================================ */
/*..cb_process_control -- process callback for CONTROL */
static void T_ENTRY cb_process_control
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR cmd;
  
  if (!TipcMsgSetCurrent(data->msg, 0)) {
    TutOut("Could not set current field of message.\n");
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcMsgNextStr(data->msg, &cmd)) {
    TutOut("Could not parse CONTROL message.\n");
    TutExit(T_EXIT_FAILURE);
  }

  if (!strcmp(cmd, "shutdown")) {
    TutOut("Process shutting down, please wait...\n");
    shutdown_in_progress = T_TRUE;
  }
  else {
    TipcCbSrvProcessControl(conn, data, arg);
  }
  
} /* cb_process_control */


/* ================================================================ */
/*..cb_process_numeric_data -- process callback for NUMERIC_DATA */
static void T_ENTRY cb_process_numeric_data
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR id_str;
  T_INT4 i;
  T_STR var_name[3];
  T_REAL8 var_value[3];
  T_REAL8 in_time, out_time;
  T_STR sender;

  in_time = TutGetWallTime();

  if (!TutTsdGetValue(id_key, (T_PTR)&id_str)) {
    TutOut("Could not get TSD value for thread.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcMsgGetSender(data->msg, &sender)) {
    TutOut("%s: Could not get message sender.\n", id_str);
    TutExit(T_EXIT_FAILURE);
  }

  if (!TipcMsgSetCurrent(data->msg, 0)) {
    TutOut("%s: Could not set current field of message.\n", id_str);
    TutExit(T_EXIT_FAILURE);
  }
  
  for (i = 0; i < 3; i++) {
    if (!TipcMsgNextStrReal8(data->msg,
                             &var_name[i], &var_value[i])) {
      TutOut("%s: Could not parse NUMERIC_DATA message.\n", id_str);
      TutExit(T_EXIT_FAILURE);
    }
  }
  
  TutOut("%s: (%s %d %s %s %s %s) from <%s>\n", id_str,
         var_name[0], (T_INT4)var_value[0],
         var_name[1], TutRealToStr(var_value[1]),
         var_name[2], TutTimeNumToStr(var_value[2]),
         sender);

  TutSleep(var_value[1]);
  
  out_time = TutGetWallTime();
  
  if (!TipcMsgWrite(data->msg,
                    T_IPC_FT_STR, "in-time",
                    T_IPC_FT_REAL8, in_time,
                    T_IPC_FT_STR, "out-time",
                    T_IPC_FT_REAL8, out_time,
                    T_NULL)) {
    TutOut("%s: Could not append fields to NUMERIC_DATA message.\n",
           id_str);
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcMsgSetDest(data->msg, sender)) {
    TutOut("%s: Could not set message destination.\n", id_str);
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcMsgSetLbMode(data->msg, T_IPC_LB_NONE)) {
    TutOut("%s: Could not set message load balancing mode.\n", id_str);
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcSrvMsgSend(data->msg, T_TRUE)) {
    TutOut("%s: Could not send NUMERIC_DATA message.\n", id_str);
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcSrvFlush()) {
    TutOut("%s: Could not flush outgoing messages to RTserver.\n",
           id_str);
  }
  
} /* cb_process_numeric_data */


/* ================================================================ */
/*..service_loop -- inner loop to receive and process requests */
void service_loop(void)
{
  T_STR id_str;

  if (!TutTsdGetValue(id_key, (T_PTR)&id_str)) {
    TutOut("Could not get TSD value for thread.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* TipcSrvMainLoop will reconnect to RTserver if errors occur. */
  do {
    if (!TipcSrvMainLoop(SHUTDOWN_CHECK_INTERVAL)) {
      TutOut("%s: TipcSrvMainLoop failed with error code %d\n",
	     id_str, TutErrNumGet());
    }
  } while (!shutdown_in_progress);
  
} /* service_loop */


/* ================================================================ */
/*..helper_thread -- thread function for helper threads */
T_PTR T_ENTRY helper_thread
(
 T_PTR arg
)
{
  T_STR id_str = (T_STR)arg;

  if (!TutTsdSetValue(id_key, id_str)) {
    TutOut("Could not set TSD value for thread.\n");
    TutExit(T_EXIT_FAILURE);
  }

  service_loop();
  
  TutOut("%s: Thread shutting down.\n", id_str);
  return T_NULL;

} /* helper_thread */


/* ================================================================ */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_BOOL have_threads;
  T_IPC_MT mt;
  T_CB cb;
  T_INT4 i, num_queued;
  T_THREAD thread[HELPER_COUNT];
  T_STRING id_strs[HELPER_COUNT];

  /* This process will use multiple threads if supported. */
  have_threads = TipcInitThreads();
  if (have_threads) {
    TutOut("Multithreaded example network service starting up.\n");
  }
  else {
    TutOut("Single-thread example network service starting up.\n");
  }

  TutCommandParseStr("setopt default_subject_prefix /rtsvcp");
  
  if (!TutTsdKeyCreate(&id_key, T_NULL_FUNC)) {
    TutOut("Could not create TSD key.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TutTsdSetValue(id_key, "M")) {
    TutOut("Could not set TSD value for main thread.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Creating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create the connection to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  }

  TutOut("Start subscribing to subject <%s>.\n", REQUEST_SUBJECT);
  if (!TipcSrvSubjectSetSubscribe(REQUEST_SUBJECT, T_TRUE)) {
    TutOut("Could not subscribe to request subject.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Replace the callback to process CONTROL messages. */
  mt = TipcMtLookupByNum(T_MT_CONTROL);
  if (T_NULL == mt) {
    TutOut("Could not lookup CONTROL message type.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  cb = TipcSrvProcessCbLookup(mt, TipcCbSrvProcessControl, T_NULL);
  if (T_NULL == cb) {
    TutOut("Could not lookup CONTROL process callback.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TutCbSetFunction(cb, (T_CB_FUNC)cb_process_control)) {
    TutOut("Could not replace CONTROL process callback.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Create a callback to process NUMERIC_DATA messages. */
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (T_NULL == mt) {
    TutOut("Could not lookup NUMERIC_DATA message type.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  if (TipcSrvProcessCbCreate(mt,
			     ((T_IPC_CONN_PROCESS_CB_FUNC)
			      cb_process_numeric_data),
			     T_NULL) == T_NULL) {
    TutOut("Could not create NUMERIC_DATA process callback.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* Start helper threads on supported platforms. */
  if (have_threads) {
    for (i = 0; i < HELPER_COUNT; i++) {
      sprintf(id_strs[i], "%d", i + 1);
      thread[i] = TutThreadCreate((T_THREAD_FUNC)helper_thread, 
                                  id_strs[i], T_NULL);
      if (TutThreadEqual(T_INVALID_THREAD, thread[i])) {
        TutOut("Could not create thread %s.\n", id_strs[i]);
        TutExit(T_EXIT_FAILURE);
      }
    }
  }

  service_loop();

  TutOut("Stop subscribing to subject <%s>.\n", REQUEST_SUBJECT);
  if (!TipcSrvSubjectSetSubscribe(REQUEST_SUBJECT, T_FALSE)) {
    TutOut("Could not stop subscribing to request subject.\n");
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Wait for any helper threads to shutdown. */
  if (have_threads) {
    for (i = 0; i < HELPER_COUNT; i++) {
      if (!TutThreadWait(thread[i], T_NULL)) {
        TutOut("Could not wait for thread %s.\n", id_strs[i]);
        TutExit(T_EXIT_FAILURE);
      }
    }
  }

  /* Take care of any late-arriving requests. */
  do {
    service_loop();
    if (!TipcSrvGetNumQueued(&num_queued)) {
      TutOut("Could not get number of queued messages.\n");
      TutExit(T_EXIT_FAILURE);
    }
  } while (num_queued > 0);
  
  TutOut("Destroying connection to server process.\n");
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Could not destroy connection to server process.\n");
    TutExit(T_EXIT_FAILURE);
  }

  return T_EXIT_SUCCESS;
  
} /* main */
